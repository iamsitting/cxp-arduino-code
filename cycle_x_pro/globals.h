//
//  globals.h
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/22/16.
//  Copyright © 2016 team14. All rights reserved.
//

#ifndef globals_h
#define globals_h

#include "types.h"

//Master MACROS
#define DEMO_CODE
//#define FINAL_CODE
//#define FINAL_CODE_NO_LEDS
//#define DEMO_CODE_NO_TRIO
//#define DEMO_CODE_ONLY_TRIO


/* Test Parameters */

//Use TEST_CODE instead of ENABLE_ADS
//ENABLE_LEDS needs ENABLE_ADS

#ifdef FINAL_CODE //use this
#define ENABLE_ALS
#define ENABLE_TRIO
#define ENABLE_RTD
#define ENABLE_LEDS
#endif

#ifdef DEMO_CODE //use this
#define TEST_CODE
#define TEST_RACE_ERPS
#define ENABLE_TRIO
#define ENABLE_ALS
#endif

#ifdef FINAL_CODE_NO_LEDS
#define ENABLE_ALS
#define ENABLE_TRIO
#define ENABLE_RTD
#endif

#ifdef DEMO_CODE_NO_TRIO
#define ENABLE_RTD
#define ENABLE_ALS
#define ENABLE_LEDS
#endif

#ifdef DEMO_CODE_ONLY_TRIO
#define TEST_CODE
#define TEST_RACE_ERPS
#define ENABLE_TRIO
#endif

//#define TEST_CODE
//#define TEST_RACE_ERPS
//#define ENABLE_ALS
//#define ENABLE_TRIO
//#define ENABLE_RTD

extern float32_t sine_test[32];
extern uint8_t ind;

/** Pin definitions **/
#ifdef TEST_CODE
#define PIN53 53
#define PIN51 51
#define PIN49 49
#endif


#define POT_PIN 7

#define USOUND_IN   	A0
#define ALSPIN3     	9
#define ALSPIN2     	10
#define ALSPIN1     	11
#define BRAKE_LT    	12
#define USOUND_LT   	13
#define ALS_BUTTON 	47
#define BATTERY A1
#define RELAY 6

#define ADS_INT1    48
#define ADS_INT2    50
#define ADS_CSB     52

#define DEBUG       Serial
#define GP20U7      Serial1
#define HC06        Serial2
#define XBPRO       Serial3

/** End Pin definitions **/

/** time formatting **/
#define SECS_PER_MIN (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY (SECS_PER_HOUR * 24L)

#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) ((_time_ % SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

#define HALF_SECOND     500
#define THREE_SECONDS   3000
#define TEN_SECONDS     10000
#define THIRTY_SECONDS  30000

/** bit-wise macros **/
#define CHECK_STATUS(var, pos) ((var) & (1<<(pos)))
#define CLEAR_STATUS(var, pos) ((var) &= ~((1) <<(pos)))
#define SET_STATUS(var, pos) ((var) |= ((1) <<(pos)))

#define RTS             0
#define NEW_SESSION     1
#define ERPS            2
#define BTCON           3
#define ERPS_ACK        4
#define POSS_ACC		    5

/** Modes **/
#define MODE_IDLE       0
#define MODE_SOLO       1
#define MODE_RACE       2
#define MODE_ATHLETE    3
#define MODE_COACH      4
#define MODE_ERPS       5

/** Protocols **/
#define SEND_IDLE       27
#define SEND_DATA       1 //Also for athelete
#define SEND_HEADER     2
#define SEND_ERPS       3
#define SEND_RACE       4
#define SEND_COACH		  5
#define SEND_HANDSHAKE  6

/** BT definitions **/
#define BAUD_RATE   	9600
#define BUFFER_SIZE 	40
#define MISSES_ALLOWED 	15
#define CXP_BYTE 0xA7

extern uint32_t g_wOffsetTime;
extern struct timeStamp g_TimeStamp;

/* Bluetooth variables */
extern uint8_t g_bySendPacket[BUFFER_SIZE];
extern uint8_t g_byRecvPacket[BUFFER_SIZE];
extern uint8_t g_byBTSendFlag;
extern uint8_t g_byBTRecvFlag;

/* Program variables */
extern uint8_t g_byNextUpdate;
extern uint8_t g_byMode;
extern uint32_t g_wIdleMillis;
extern uint32_t g_wDataMillis;
extern uint8_t g_byStatus;
extern uint8_t g_byMisses;
/* Bitfield variable
 Bit 0: Ready To Send - Only sends data when RTS is high. RTS goes low after sending data.
 Bit 1: New Session - When high CSV header is sent instead of data.
 Bit 2: ERPS - This is high when ERPS mode is enabled.
 Bit 3: BTCON - This is high when BT connection is established
 Bit 4: ERPS_ACK - This is high when ERPS mode is enabled on app.
 Bit 5: POSS_ACC - This is high when ADS detects possible accident
 Bit 6: Undefined
 Bit 7: Undefined
 */


#endif /* globals_h */
