# HighPrecPosSens

High precision Position Sensor System using
* Hamamatsu s12443, 
* a gradient lens array,
* a paper or metal stripe / code encoder scale 
* an RP2040 or Teensy 4.1 µC board.

## s12443

Pixel size: 7 x 125 μm, 2496 pixels, small package

The S12443 is a CMOS linear image sensor with a compact yet 2496-pixel long photosensitive area (effective photosensitive area length: 17.472 mm). The pixel size is 7 x 125 μm.

* Features
* Pixel size: 7 × 125 μm
* 2496 pixels
* Effective photosensitive area length: 17.472 mm
* 3.3 V single power supply operation
* Built-in timing generator allows operation with only Start and Clock pulse inputs
* Video data rate: 10 MHz max.
* Small imput terminal capacitance: 5 pF

In my experiments I saw that 10MHz usage is leading to higher noise levels.

I recommend 5 MHz pxel clock signal, thats nicely representing the measured analog signal.



## Credits to

https://github.com/ncarandini/KiCad-RP-Pico.git

