use clap::Parser;

/// Software implementation of image processing for background blurring
#[derive(Parser, Debug)]
#[command(version, about)]
pub struct Args {
    /// Input file to read video from
    #[arg(short, long)]
    pub input_file: String,
    /// Output file to write processed video to
    #[arg(short, long)]
    pub output_file: Option<String>,
}
