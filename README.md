# Offloading Background Effects to an FPGA

As a Linux user, I noticed that most video conferencing applications did not support algorithms for background blurring or background replacement. My first attempt at fixing this issue was running the algorithms on my machine. This did work when I was running the algorithms on my desktop with a 3080, but my laptop had trouble keeping up with the frame rate of my webcam (30 fps). So, I thought this could be optimized by offloading this processing to an FPGA that would be embedded into a webcam so that there is no burden on the user's machine. It is important to note that this problem has been mostly solved. Video conferencing applications have mostly moved to PWAs that can leverege whatever backend the provider has to run these algorithms instead of a client machine. However, this is an interesting project to me since I would be touching topics from hardware and PCB design, FPGA design and optimization, and firmware.
