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
/* Test Parameters */
#define TEST_CODE
#define TEST_ISR
#define ALS_TEST
//#define TEST_ERPS

/** Pin definitions **/
#ifdef TEST_CODE
#define PIN53 53
#define PIN51 51
#define PIN49 49
#endif


#define POT_PIN 7

#define USOUND_IN   A0
#define ALSPIN3     9
#define ALSPIN2     10
#define ALSPIN1     11
#define BREAK_LT    12
#define USOUND_LT   13
#define ALS_BUTTON_PIN 47

#define ADS_INT1    48
#define ADS_INT2    50
#define ADS_CSB     52

#define DEBUG       Serial
#define GP20U7      Serial1
#define HC06        Serial2
#define XBPRO       Serial3


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

/** Modes **/
#define MODE_IDLE       0
#define MODE_SOLO       1
#define MODE_RACE       2
#define MODE_TRAINEE    3
#define MODE_TRAINER    4
#define MODE_ERPS       5

/** Protocols **/
#define SEND_IDLE    27
#define SEND_DATA       1
#define SEND_HEADER     2
#define SEND_ERPS       3

/** BT definitions **/
#define BAUD_RATE   9600
#define BUFFER_SIZE 32


/** ALS variables **/
#define DEBOUNCE_DELAY 200
#define FLASH_INTERVAL 100

extern uint8_t g_byChangedToSimple;
extern uint32_t g_wLastDebounceTime;
extern uint8_t g_byFlashingPattern;
extern uint32_t g_wPreviousMillis;
extern uint8_t g_byALSPin1State;
extern uint8_t g_byALSPin2State;
extern uint8_t g_byALSPin3State;
extern uint8_t g_byFlashingCount;


/* Metric variables */
extern union Floater32_t g_fMetric1;
extern union Floater32_t g_fMetric2;
extern union Floater32_t g_fMetric3;
extern uint32_t g_wOffsetTime;
extern struct timeStamp g_TimeStamp;
extern union Floater32_t g_fLatitude;
extern union Floater32_t g_fLongitude;

extern uint8_t g_byBatteryLevel;
extern uint8_t g_byThreat;

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
/* Bitfield variable
 Bit 0: Ready To Send - Only sends data when RTS is high. RTS goes low after sending data.
 Bit 1: New Session - When high CSV header is sent instead of data.
 Bit 2: ERPS - This is high when ERPS mode is enabled.
 Bit 3: BTCON - This is high when BT connection is established
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
 Bit 4: Undefined
 Bit 5: Undefined
 Bit 6: Undefined
 Bit 7: Undefined
 
 */


/** main functions **/
void btListen(void);
void btSend(void);
void setupALS(void);


#endif /* globals_h */
