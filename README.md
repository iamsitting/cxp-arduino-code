## Synopsis

This project contains the embedded software for the Cycle X-Pro. This project is designed for an Aduino DUE, although, the majority of the code is written in C++ rather than a single INO file.

## Log

### Version

3.0.7

### Most Recent Changes

3.0.7
- Tested AT commands
- Added New Xbee settings
- Added handshake to protocol

3.0.6
- Added AT Command Functions (Untested)

3.0.5
- Integrated Power software (Untested)
- Added BatteryLevel Reading
- Added Generator Relay Control

3.0.4
- GPS Tested
- Location and Speed tested

3.0.3
- ALS Integration tested
- ADS/ERPS Integration tested
- Lat & Long come out to 0

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
