## Synopsis

This project contains the embedded software for the Cycle X-Pro. This project is designed for an Aduino DUE, although, the majority of the code is written in C++ rather than a single INO file.

## Log

### Version

3.0.3

### Most Recent Changes

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

2.2.0
- Added TRIO functionality - untested

2.1.0
- Added Race Mode functionality - untested

2.0.11
- Threat flag added, not tested
- New BT receiving routine

## Code Design

The "main" code is located in the INO file. Features are to be added to the functions file.

### types.h

This file defines types, structures, and unions that are to be used in the program. This is done to make programs efficient and arduino independent.

### globals.h

This file defines global variables that are used across the INO and C files. Descriptions for bitfield variables are located here.

### functions.cpp

This is where data processing functions and futrue functions are to be written. Function declarations are located in the H file.

### als.cpp

This is where ALS functions are written.

### trio.cpp

This is where TRIO adn XBee functions are written.

### rtd.cpp

This is where RTD and ADS functions are written.
