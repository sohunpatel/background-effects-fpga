mod cli;
mod io;
mod processing;

extern crate ffmpeg_next as ffmpeg;

use clap::Parser;
use ffmpeg::format::input;
use ffmpeg::media::Type;
use ffmpeg::software::scaling::{context::Context, flag::Flags};
use ffmpeg::util::frame::video::Video;
use std::sync::mpsc::{Receiver, Sender};
use std::thread;

use processing::pipeline;

fn main() -> Result<(), ffmpeg::Error> {
    // parse CLI arguments
    let args = cli::Args::parse();
    // initialize the ffmpeg library
    ffmpeg::init().unwrap();

    // check if the input valid is valid and create its context
    if let Ok(mut ictx) = input(&args.input_file) {
        // get video stream from input
        let input = ictx
            .streams()
            .best(Type::Video)
            .ok_or(ffmpeg::Error::StreamNotFound)
            .unwrap();
        // keep track of index of stream for video channel for reference through application
        let video_stream_index = input.index();

        // generate a decoder that can convert compressed frames to full frames
        let context_decoder =
            ffmpeg::codec::context::Context::from_parameters(input.parameters()).unwrap();
        let mut decoder = context_decoder.decoder().video().unwrap();

        // channel to send the writer a frame to write to disk
        let (write_tx, write_rx): (Sender<io::FrameMessage>, Receiver<io::FrameMessage>) =
            std::sync::mpsc::channel();
        // this channel is used to tell threads to terminate
        let (control_tx, control_rx): (Sender<i32>, Receiver<i32>) = std::sync::mpsc::channel();

        // holds all thread handles
        let mut threads = Vec::new();

        // spawn thread to write frames to disk
        threads.push(thread::spawn(move || {
            io::write::write_consumer(write_rx, control_rx);
        }));

        // spawn thread that reads from file and sends frames to decoder
        threads.push(thread::spawn(move || {
            // contain thread handles
            let mut threads = Vec::new();
            // generate scaler to convert frames to RGB24 pixel format
            let mut scaler = Context::get(
                decoder.format(),
                decoder.width(),
                decoder.height(),
                ffmpeg::format::Pixel::RGB24,
                decoder.width(),
                decoder.height(),
                Flags::BICUBIC,
            )
            .unwrap();
            // iterate through packets and streams
            for (stream, packet) in ictx.packets() {
                // only run through data stream for video
                if stream.index() == video_stream_index {
                    // send frame to decoder
                    decoder.send_packet(&packet).unwrap();
                    let mut decoded = Video::empty();
                    while decoder.receive_frame(&mut decoded).is_ok() {
                        let mut rgb_frame = Video::empty();
                        scaler.run(&decoded, &mut rgb_frame).unwrap();
                        let write_tx = write_tx.clone();
                        threads.push(thread::spawn(move || {
                            pipeline::pipeline(&mut rgb_frame, write_tx);
                        }));
                    }
                }
            }
            // send the decoder EOF
            decoder.send_eof().unwrap();
            // decode handle the EOF
            let mut decoded = Video::empty();
            while decoder.receive_frame(&mut decoded).is_ok() {
                let mut rgb_frame = Video::empty();
                scaler.run(&decoded, &mut rgb_frame).unwrap();
                let write_tx = write_tx.clone();
                threads.push(thread::spawn(move || {
                    pipeline::pipeline(&mut rgb_frame, write_tx);
                }));
            }
            // wait the thread to make sure that the EOF is handled
            thread::sleep(std::time::Duration::from_secs(1));
            // wait for thread to terminate
            for thread in threads {
                thread.join().expect("Processing thread error");
            }
            // we are done, so send the number of frame to the control fifo
            control_tx.send(200).unwrap();
        }));

        // join threads
        for thread in threads {
            thread.join().expect("Thread error");
        }
    } else {
        println!("Cannot open input file: {}", args.input_file);
    }

    Ok(())
}
