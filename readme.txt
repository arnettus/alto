Paper on Hartley Transforms as an audio compression algorithm:
    >> thesai.org/Downloads/Volume8No4/Paper_62-DSP_Real-Time_Implementation_of_an_Audio.pdf

Paper on the mathematics behind the Fast Hartley Transform:
    >> sep.stanford.edu/data/media/public/oldreports/sep38/38_29.pdf

Reference manual for the Teensy 3.2's Cortex-M4 microcontroller (MK20DX256VLH7):
    >> www.nxp.com/docs/en/reference-manual/K20P64M72SF1RM.pdf

Reference for voice frequency:
    >> https://en.wikipedia.org/wiki/Voice_frequency

Reference for FFT frequency bins:
    >> https://electronics.stackexchange.com/questions/12407/what-is-the-relation-between-fft-length-and-frequency-resolution

Sampling rates
--------------
Telephones use a range of 300Hz to 3400Hz. According to the Nyquist-Shannon theorem, a sampling of
6800Hz will suffice. For good quality speech, a sampling rate of 8kHz is recommended.

Frequency bin resolution is calculated by BRes = Fs / N, where Fs = sampling freq. and N is the number of
data points.

Memory
------
The Teensy 3.2 has 64kB of RAM.
If 256 data points is chosen, then BRes = 31.25.
If each data point is processed as a float, then 256*4 = 1024 bytes are needed per FHT transform.
The recursive implementation will approximately double this, so 2048 bytes.

Based off research on human speech, we aim to capture a range of 80Hz - 4000Hz.
4000 - 80 = 3920. 3920 / 31.25 = 126. 126 outputs will represent the 80Hz - 4000Hz range.
126 * 4 = 504 bytes (before thresholding) will be used per audio packet.
Thresholding will reduce this size.

High-level Architecture
-----------------------
ADC -> DMA -> buffer -> FHT -> thresholding -> packet formation -> UART

