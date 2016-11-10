## Synopsis

This project contains the embedded software for the Cycle X-Pro. This project is designed for an Aduino DUE, although, the majority of the code is written in C++ rather than a single INO file.

## Log

### Version

3.0.4

### Most Recent Changes

3.0.4
- GPS Tested
- Location and Speed tested

3.0.3
- ALS Integration tested
- ADS/ERPS Integration tested
- Lat & Long come out to 0

3.0.2
- Edited for consisten data types

3.0.1
- Added Name/Weight Parameters (Untested)
- Reorganized functions

3.0.0
- Integrated RTD & ADS (Untested)
- Added TinyGPS++ and BMI160 libraries
- Integrated TRIO (Untested)
- Integrated ALS, Threat and Brake feature (Untested)
- Switched from C to C++ files

2.2.1
- Race Mode Tested
- Count misses for reliability

## Code Design

The "main" code is located in the INO file. Features are to be added to the functions file.

Project Structure:
```
├── cycle_x_pro/
│   ├── types.h
│   │   ├── i.e uint8_t, Floater32_t,  etc.
│   ├── globals.h
│   │   ├── i.e pin defs, system vars, timers etc.
│   ├── functions.h
│   ├── functions.cpp
│   │   ├── i.e BluetoothSend(),  BluetoothReceive(), etc.
│   ├── als.h
│   ├── als.cpp
│   │   ├── i.e g_byBatteryLevel, switchFlashingPattern(), etc.
│   ├── rtd.h
│   ├── rtd.cpp
│   │   ├── i.e g_fSpeed, g_byGPSMisses, getSpeed(), etc.
│   ├── trio.h
│   ├── trio.cpp
│   │   ├── i.e g_byXbeeSendPacket, XBeeSendMessage(), etc.

```
