## Synopsis

This project contains the embedded software for the Cycle X-Pro. This project is designed for an Aduino DUE, although the majority of the code is written in C rather than a single INO file.

## Log

### Version

2.0.9

### Most Recent Changes

2.0.9
- ALS is tested
- Testing Battery Indicator on SEND_DATA

2.0.8
- Button Interrupt tested

2.0.7
- Bugfix: ALS Flashing Patter 3&4
- ERPS is fully tested

2.0.6
- Reorganized ALS code
- Rewrote byteWrite more intuitively

2.0.5
- Updated Pin Mappings
- bugfix: Was not sending ERPS message

## Code Design

The "main" code is located in the INO file. Features are to be added to the functions file.

### types.h

This file defines types, structures, and unions that are to be used in the program. This is done to make programs efficient and arduino independent.

### globals.h

This file defines global variables that are used across the INO and C files. Descriptions for bitfield variables are located here.

### functions.c

This is where data processing functions and futrue functions are to be written. Function declarations are located in the H file.

### testing.c

This is where untested or feedback functions are written. Function declarations are located in the H file. Macros are in the globals file.

### als.c

This is where ALS functions are written.
