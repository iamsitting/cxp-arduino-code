#ifndef TYPES_H
#define TYPES_H

//8-bit integer, may also use char and byte
typedef		signed		char	int8_t;
typedef		unsigned	char	uint8_t;

//16-bit integer, may also use short and word
//typedef		signed 		short	int16_t;
//typedef		unsigned	short	uint16_t;

//32-bit integer
typedef		signed		long	int32_t;
typedef		unsigned	long	uint32_t;

//32-bit floating-point
typedef					float	float32_t;

//data structures
struct TwoWords {
	uint16_t rd0;
	uint16_t rd1;
};

struct fourbytes {
	uint8_t te0;
	uint8_t te1;
	uint8_t te2;
	uint8_t te3;
};

//Floater type
union Floater32_t{
	float32_t bits32;
	TwoWords wo;
	fourbytes by;
};

struct timeStamp {
  uint8_t hour;
  uint8_t minute;
  Floater32_t second;
};



#endif
