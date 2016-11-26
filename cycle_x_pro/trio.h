//
//  trio.h
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 10/27/16.
//  Copyright © 2016 team14. All rights reserved.
//

#ifndef trio_h
#define trio_h

#include "globals.h"
#include "types.h"

#define XBEE_BUFFER_SIZE 32
#define XBEE_BAUD 9600
#define START_BYTE 0x7E
#define NAME_SIZE 10
#define ADDR_SIZE 8
#define GUARD_TIME 1000


#define TRIO_SOLO 0
#define TRIO_ATHLETE 1
#define TRIO_COACH 2
#define TRIO_RACE 3
#define TRIO_READY 4
#define TRIO_INIT 5

#define TRIO_ERPS_0 0
#define TRIO_ERPS_1 1

/*
TRIO = 0 SOLO
TRIO = 1 TRAIN - ATHLETE
TRIO = 2 TRAIN - COACH
TRIO = 3 RACE
TRIO = 4 Ready
TRIO = 5 Initial Configuration
*/

extern uint8_t g_byMyTRIOid[ADDR_SIZE];
extern uint8_t g_byDestTRIOid[ADDR_SIZE];
extern uint8_t g_byTRIOisInit;
extern uint8_t g_byTRIOisReady;

extern uint8_t g_byXbeeRecvPacket[XBEE_BUFFER_SIZE];
extern uint8_t g_byXbeeSendPacket[XBEE_BUFFER_SIZE];
extern uint8_t g_byXbeeSendFlag;
extern uint8_t g_byXbeeRecvFlag;
extern uint8_t g_byXbeeisConfig;
extern uint8_t g_byXbeemisses;

extern union Floater32_t g_fOppSpeed;
extern union Floater32_t g_fOppDistance;
extern union Floater32_t g_fOppCalories;
extern union Floater32_t g_fOppLongitude;
extern union Floater32_t g_fOppLatitude;
extern uint8_t g_fOppERPS;
extern uint8_t g_byUserName[NAME_SIZE];
extern uint8_t g_byOppUserName[NAME_SIZE];
extern uint8_t g_byWinning;
extern uint8_t g_byReadyCount;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    
    
    
    /** TRIO Functions **/
    void XBeeDeconstructMessage(void);
    void XBeeBuildMessage(uint8_t protocol);
    void XbeeSendMessage(void);
    void XBeeReceive(void);
    void setupTrio(void);
    void countMiss(void);

    /** XBEE Commands **/
    uint8_t ATcheckOK(void);
    uint8_t ATenterCommand(void);
    uint8_t ATDH(uint8_t addr);
    uint8_t ATDL();
    uint8_t ATDL_READ();
    uint8_t ATMY(uint8_t addr);
    uint8_t ATWR(void);
    uint8_t ATCN(void);
    uint8_t ATSL(void);
    uint8_t XBeeATSL(void);
    uint8_t XBeeATHL(void);
    uint8_t XBeeConfigure(void);

    
    
    
    
#ifdef __cplusplus
}
#endif //__cplusplus


#endif /* trio_h */
