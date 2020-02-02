# Makitronics AutoFlush
Open-source Automatic hands-free flusher for residential toilets

## Overview ##
Toilets around the world are flushed every day. However, not all toilets are made equal. For example, public restrooms occasionally feature an **automatic flush mechanism** that allows for hands-free toilet flushing that is more sanitary than pushing a level or handle.

At home, many people such as children or elderly may face the same hesitancy with flushing due to frequent contact with a toilet that may have germs. Additionally, it can be a hassle to perform a complete flush of the toilet (by holding the lever down for more than 6 seconds). Commercial or name-brand automatic flushing systems cost hundreds of dollars and require expert installation. Since most residential toilets have flapper-style or canister mechanism for flushing, we have created one of the world's first open-source hands-free toilet flusher.

The Makitronics AutoFlush is an open-source customizable system that can easily be mounted to almost any residential toilet. It has presence-checking algorithms to detect for human presence similar to commercial automatic flushing devices. You can customize the distance of detection, minimum detection time, absence countdown, flush duration, and tank refill duration. Once configured, the device is very reliable. Long-term consistent flushing could prevent toilet clogs to a certain extent (no guarantees).

## Requirements ##
If you would like to build **AutoFlush v1**, you will need the following:

- Arduino Nano
- 5x Individual Neopixels WS2812
- Sharp GP2Y0A21 IR sensor
- 55G Metal Gear Servo
- Long USB cable (length from the nearest GFCI to your toilet)
- USB Adapter
- AutoFlush v1 3D printed parts
- Firmware code

If you would like to build **AutoFlush v2**, you will need the following:

- Heltec WiFi Kit 8
- Sharp GP2Y0A21 IR sensor
- 55G Metal Gear Servo
- Long USB cable (length from the nearest GFCI to your toilet)
- USB Adapter
- AutoFlush v2 3D printed parts
- Firmware code

**Compatibility:** This design is intended for residential toilets that have flapper-style or canister mechanism. It is not for dual-flush, ballcock valve, top-mount flush and pressure-assist toilets!

**Disclaimer:** This project provides resources AS-IS and does not guarantee any compatibility with your toilet. No warranty is provided. By building this project, you acknowledge that you have basic knowledge of electronics and agree to take responsibility for any damages or injury at your own risk. You agree that the author will not be liable for any incompatibilities, damages, or injury as a result of this project.

## Tutorial ##
This is a work in progress. Tutorials will come out soon AutoFlush v1 is very reliable and colorful but requires hard-coded configurations. For that reason, it may be depreciated soon. AutoFlush v2 is a means to provide a smaller form-factor and better user interface. 