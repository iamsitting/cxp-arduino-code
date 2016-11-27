## Synopsis

This project contains the embedded software for the Cycle X-Pro. This project is designed for an Aduino DUE, although, the majority of the code is written in C++ rather than a single INO file.

## Log

### Version

3.0.15

### Most Recent Changes

3.0.15
- Tested Incident Outreach
- bugfix: TRIO Endianess

3.0.14
- Added I.O.

3.0.13
- Train Mode Tested
- Race Mode Tested (Again)

3.0.12
- bugfix: More Race Mode bugs

3.0.11
- bugfix: Handshake and Idle protocol message bug

3.0.10
- Added state of charge
- Added Coach mode to protocol

3.0.9
- Updated AT Commands

3.0.8
- Race Mode Tested

3.0.7
- Tested AT commands
- Added New Xbee settings
- Added handshake to protocol

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
