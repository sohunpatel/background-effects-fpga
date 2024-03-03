use ffmpeg::util::frame::video::Video;
use itertools::izip;
use std::sync::mpsc::Sender;

use crate::io::FrameMessage;

fn image_difference(minuend: &mut Video, subtrahend: &mut Video) -> Video {
    let mut difference = Video::empty();

    let minuend_data = minuend.data_mut(0);
    let subtrahend_data = subtrahend.data_mut(0);
    let difference_data = difference.data_mut(0);

    for (m, s, d) in izip!(
        minuend_data.chunks_mut(3),
        subtrahend_data.chunks_mut(3),
        difference_data.chunks_mut(3)
    ) {
        d[0] = m[0] - s[0];
        d[1] = m[1] - s[1];
        d[2] = m[2] - s[2];
    }

    difference
}

pub fn pipeline(frame: &mut Video, write_tx: Sender<FrameMessage>) {
    let data = frame.data_mut(0);
    for p in data.chunks_mut(3) {
        p[0] = 0;
        p[2] = 0;
    }
    write_tx
        .send(FrameMessage {
            frame: frame.clone(),
            index: 0,
        })
        .unwrap();
}
