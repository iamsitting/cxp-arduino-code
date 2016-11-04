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

#define TRIO_INIT 3
#define TRIO_TRAIN 1
#define TRIO_RACE 2

#define TRIO_ERPS_0 0
#define TRIO_ERPS_1 1

#define TRIO_ATHLETE 0
#define TRIO_COACH 1

extern uint8_t g_byXbeeRecvPacket[XBEE_BUFFER_SIZE];
extern uint8_t g_byXbeeSendPacket[XBEE_BUFFER_SIZE];
extern uint8_t g_byXbeeSendFlag;
extern uint8_t g_byXbeeRecvFlag;

extern union Floater32_t g_fOppSpeed;
extern union Floater32_t g_fOppDistance;
extern union Floater32_t g_fOppLongitude;
extern union Floater32_t g_fOppLatitude;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    
    
    
    /** TRIO Functions **/
    void XBeeDeconstructMessage(void);
    void XBeeBuildMessage(uint8_t protocol);
    void XBeeSendMessage(void);
    void setupTrio(void);
    
    
#ifdef __cplusplus
}
#endif //__cplusplus


#endif /* trio_h */
