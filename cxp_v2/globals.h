#ifndef GLOBALS_H
#define GLOBALS_H
#include "types.h"

	/** Pin definitions **/
#define POT 7

	/** time formatting **/
#define SECS_PER_MIN (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY (SECS_PER_HOUR * 24L)

#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) ((_time_ % SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

	/** Modes **/
#define SEND_DATA 1
#define SEND_HEADER 2
	
	/** BT definitions **/
#define BAUD_RATE 9600
	
	/** global variables **/
extern Floater32_t g_fMetric1;
extern Floater32_t g_fMetric2;
extern Floater32_t g_fMetric3;
extern uint8_t sf; //status field

	/** functions **/
int8_t btListen(void);
void btSend(void);
void getData(void);
void getTime(void);
void byteWrite(uint8_t protocol);
#endif
