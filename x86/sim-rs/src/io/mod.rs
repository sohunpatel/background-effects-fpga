pub mod read;
pub mod write;

use ffmpeg::util::frame::video::Video;

pub struct FrameMessage {
    pub frame: Video,
    pub index: i32
}
