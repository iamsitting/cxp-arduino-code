## Synopsis

This project contains the embedded software for the Cycle X-Pro. This project is designed for an Aduino DUE, although the majority of the code is written in C rather than a single INO file.

## Log

### Version

2.0.5

### Most Recent Changes

2.0.5
- Updated Pin Mappings
- bugfix: Was not sending ERPS message

2.0.4
- Added battery protocol and idle mode
- ERPS protocool and mode not working

2.0.3
- Edited for Due. BT tested. ERPS untested

2.0.2 - Added testing header files for test/validation
- Loadable, not tested

## Code Design

The "main" code is located in the INO file. Features are to be added to the functions file.

### types.h

This file defines types, structures, and unions that are to be used in the program. This is done to make programs efficient and arduino independent.

### globals.h

This file defines global variables that are used across the INO and C files. Descriptions for bitfield variables are located here.

### functions.c

This is where data processing functions and futrue functions are to be written. Function declarations are located in the H file.

