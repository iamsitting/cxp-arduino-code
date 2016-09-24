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

/** Pin definitions **/
#define POT 7
#define ALSPIN1 31
#define ALSPIN2 33
#define ALSPIN3 35

/** time formatting **/
#define SECS_PER_MIN (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY (SECS_PER_HOUR * 24L)

#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) ((_time_ % SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

#define TEN_SECONDS 10000
#define THIRTY_SECONDS 30000

/** Modes **/
#define MODE_IDLE 0
#define MODE_SOLO 1
#define MODE_RACE 2
#define MODE_TRAINEE 3
#define MODE_TRAINER 4
#define MODE_ERPS 5

/** bit-wise macros **/
#define CHECK_STATUS(var, pos) ((var) & (1<<(pos)))
#define CLEAR_STATUS(var, pos) ((var) &= ~((1) <<(pos)))
#define SET_STATUS(var, pos) ((var) |= ((1) <<(pos)))

#define RTS 0
#define NEW_SESSION 1
#define ERPS 2
#define BTCON 3

/** Protocols **/
#define SEND_BATTERY 27
#define SEND_DATA 1
#define SEND_HEADER 2
#define SEND_ERPS 3

/** BT definitions **/
#define BAUD_RATE 9600
#define BUFFER_SIZE 32
#define HC06 Serial3


/** ALS variables **/
#define DEBOUNCE_DELAY 50

#define FLASH_INTERVAL 200

extern uint8_t g_byLastButtonState;
extern uint8_t g_byCurrentButtonState;
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

/* Bluetooth variables */
extern uint8_t g_bySendPacket[BUFFER_SIZE];
extern int8_t g_byRecvPacket;

/* Program variables */
extern uint8_t g_byNextUpdate;
extern uint8_t g_byMode;
extern uint32_t g_wIdleMillis;
extern uint8_t g_byStatus;
/* Bitfield variable
 Bit 0: Ready To Send - Only sends data when RTS is high. RTS goes low after sending data.
 Bit 1: New Session - When high CSV header is sent instead of data.
 Bit 2: Stream - Only sends data when this bit high. BT Client can toggle this on and off.
 Bit 3: ERPS - This is high when ERPS mode is enabled.
 Bit 4: BTCON - This is high when BT connection is established
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
int8_t btListen(void);
void btSend(void);


#endif /* globals_h */
