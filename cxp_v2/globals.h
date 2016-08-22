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
#define BUFFER_SIZE 32
	
	/** global variables **/
	
	/* Metric variables */
extern union Floater32_t g_fMetric1;
extern union Floater32_t g_fMetric2;
extern union Floater32_t g_fMetric3;
extern struct timeStamp g_TimeStamp;

	/* Bluetooth variables */
extern uint8_t g_bySendPacket[BUFFER_SIZE];
extern int8_t g_byRecvPacket;

	/* Program variables */
extern uint16_t g_wOffsetTime;
extern uint8_t g_byStatus;
	/* Bitfield variable
	Bit 0: Ready To Send - Only sends data when RTS is high. RTS goes low after sending data.
	Bit 1: New Session - When high CSV header is sent instead of data.
	Bit 2: Stream - Only sends data when this bit high. BT Client can toggle this on and off.
	Bit 3: ERPS - This is high when ERPS mode is enabled. 
	Bit 4: Undefined
	Bit 5: Undefined
	Bit 6: Undefined
	Bit 7: Undefined
	*/

//extern uint8_t g_byIOstatus; //bitfield
	/*	
	Bit 0: ACC. - High when Accelerometer is connected (always)
	Bit 1: Bluetooth - High when bluetooth is connected
	Bit 2: GPS - Low when GPS signal is connected
	Bit 3: XBee - High when Xbee is connected
	Bit 4: Solo Mode - High when Solo mode
	Bit 5: Race Mode - High when Race mode
	Bit 6: Train Mode - High when Train mode
	Bit 7: Undefined
	
	*/

	/** functions **/
int8_t btListen(void);
void btSend(void);
#endif
