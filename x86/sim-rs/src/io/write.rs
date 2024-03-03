use std::io::prelude::*;
use std::sync::mpsc::Receiver;

use super::FrameMessage;

pub fn write_consumer(rx: Receiver<FrameMessage>, control: Receiver<i32>) {
    let mut counter = 0;

    loop {
        match control.try_recv() {
            Ok(_) => {
                break;
            }
            Err(std::sync::mpsc::TryRecvError::Empty) => {
                if let Ok(frame_message) = rx.recv() {
                    let mut file =
                        std::fs::File::create(format!("tmp/frame{}.ppm", counter)).unwrap();
                    file.write_all(
                        format!(
                            "P6\n{} {}\n255\n",
                            frame_message.frame.width(),
                            frame_message.frame.height()
                        )
                        .as_bytes(),
                    )
                    .unwrap();
                    file.write_all(frame_message.frame.data(0)).unwrap();
                    counter += 1;
                }
            }
            Err(std::sync::mpsc::TryRecvError::Disconnected) => {
                println!("Channel is disconnected");
            }
        }
    }
}
