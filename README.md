## Synopsis

This project contains the embedded software for the Cycle X-Pro. This project is designed for an Aduino DUE, although the majority of the code is written in C rather than a single INO file.

## Log

### Version

2.0.1

### Most Recent Changes

2.0.1 - Preliminary ERPS mode added
      - Status checking is made more readable
      - Modes Added
      - Untested Commit

2.0.0 - The SW was reorganized into a few C and H files. This reorganization has made it easier to test on other arduinos and to add new features or protocols.

## Code Design

The "main" code is located in the INO file. Features are to be added to the functions file.

### types.h

This file defines types, structures, and unions that are to be used in the program. This is done to make programs efficient and arduino independent.

### globals.h

This file defines global variables that are used across the INO and C files. Descriptions for bitfield variables are located here.

### functions.c

This is where data processing functions and futrue functions are to be written. Function declarations are located in the H file.

## License

This file is licensed under MIT
 
The MIT License (MIT)
 
Copyright (C) 2016 Carlos Salamanca (@iamsitting)
 
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy modify, merge publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
