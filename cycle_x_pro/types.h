//
//  types.h
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/22/16.
//  Copyright © 2016 team14. All rights reserved.
//

#ifndef types_h
#define types_h

#include <stdint.h>

/* 8-bit integer, may also use char and byte */
typedef		signed		char	int8_t;
typedef		unsigned	char	uint8_t;

/* 16-bit integer, may also use short and word */
//for Arduino u/int16_t defined in <stdint.h>
//typedef		signed 		short	int16_t;
//typedef		unsigned	short	uint16_t;

/* 32-bit integer */
typedef		signed		long	int32_t;
typedef		unsigned	long	uint32_t;

/* 32-bit floating-point */
typedef					float	float32_t;

/** Data Structures **/

/* TwoWord to use for 32-bit types */
struct TwoWords {
    uint16_t rd0;
    uint16_t rd1;
};

/* fourbytes to use for 32-bit types */
struct fourbytes {
    uint8_t te0;
    uint8_t te1;
    uint8_t te2;
    uint8_t te3;
};

/* Breaks float into words and bytes */
union Floater32_t{
    float32_t bits32;
    struct TwoWords wo;
    struct fourbytes by;
};

/* timeStamp consists of hour. minute, and second */
struct timeStamp {
    uint8_t hour;
    uint8_t minute;
    union Floater32_t second;
};



#endif /* types_h */
