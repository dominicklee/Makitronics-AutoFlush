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

![](https://raw.githubusercontent.com/dominicklee/Makitronics-AutoFlush/master/AutoFlush%20v1/Main%20Image.jpg)

If you would like to build **AutoFlush v2**, you will need the following:

- Heltec WiFi Kit 8
- Sharp GP2Y0A21 IR sensor
- 55G Metal Gear Servo
- Long USB cable (length from the nearest GFCI to your toilet)
- USB Adapter
- AutoFlush v2 3D printed parts
- Firmware code

![](https://raw.githubusercontent.com/dominicklee/Makitronics-AutoFlush/master/AutoFlush%20v2/Images/Activation.jpg)

**Compatibility:** This design is intended for residential toilets that have flapper-style or canister mechanism. It is not for dual-flush, ballcock valve, top-mount flush and pressure-assist toilets!

**Disclaimer:** This project provides resources AS-IS and does not guarantee any compatibility with your toilet. No warranty is provided. By building this project, you acknowledge that you have basic knowledge of electronics and agree to take responsibility for any damages or injury at your own risk. You agree that the author will not be liable for any incompatibilities, damages, or injury as a result of this project.

## About AutoFlush Iterations ##
This is a document work in progress. AutoFlush v1 started off as a very reliable design and iteration but due to the lack of wireless capabilities, it required hard-coded configurations. To make AutoFlush more user-friendly, AutoFlush v2 was developed as a means to provide a smaller form-factor, more versatile device, and better user interface. The v2 unit allows an end-user to easily configure detection distance and other settings mentioned in Overview. With the exception of WS2812, the AutoFlush v2 has all the features of AutoFlush v1 and more. As we see more use-cases in the community, AutoFlush will continue to have more enhanced features.

## AutoFlush v2 Setup Guide ##
1. **Assembly:** Obtain the parts as mentioned in the **Requirements** section and build the v2 unit. Optionally, you can email the author to purchase parts/plans or an assembled unit. For questions or purchases, the author can be reached at *dominick(@)makitronics.com*
2. **Upload Firmware:** Plug in your unit. Select `NodeMCU v1.0` in the Arduino IDE. Download the dependency libraries as needed. Flash the code in `AutoFlush v2\AutoFlush_v2\AutoFlush_v2.ino`. An app will be released in the future to make this process easier.
3. **Go to Configuration:** You will need to connect AutoFlush to your home WiFi network. This will enable AutoFlush to show you the time and future features. In the same process, you will specify the settings to be used when flushing the toilet such as distance, duration, etc. To go into config mode, power on the device and open the **Serial Monitor** and then type in **config** then hit enter. 
4. **Set WiFi and settings:** Unplug and re-plug your device. Connect to the WiFi network called **AutoFlush Config** using password **12345678**. Connect to that and navigate your browser to **192.168.4.1** and click the first button to scan for WiFi networks. Type in your WiFi password. For the additional settings, specify your preferences.

	Use **numbers only** for these settings. We used the following settings by default:
	- Distance detection(cm): 80
	- Min Detect Duration (sec): 3
	- Flush Duration (sec): 5
	- Refill Duration (sec): 25
	- Normal Angle (deg): 0
	- Flush Angle (deg): 70

	You may have to experiment with some of thse settings to get the best configuration.
5. Once you submit these settings, your AutoFlush device will automatically connect to your WiFi network to acquire the current time and timezone. 
6. **Last step:** Go and 3D print the `ToiletServo.stl` file from the `AutoFlush v1` folder. Put some hot glue on the back of your servo and mount inside the tank of your toilet. You may want to wear gloves for this step. Use an extended hook or paper clips to latch onto the "chain" which pulls the flap inside the toilet. You may now walk up to the toilet and "do your business". Each stage of the detection, activation, flush, and refill will reflect the settings you specified.