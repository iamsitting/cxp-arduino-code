//
//  als.h
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/29/16.
//  Copyright © 2016 team14. All rights reserved.
//

#ifndef als_h
#define als_h

#include "globals.h"
#include "types.h"

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

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    
    
    
    /** ALS Functions **/
    void readDebounceButton(void);
    void switchFlashingPattern(void);
    void flashingPattern0(void);
    void flashingPattern1(void);
    void flashingPattern2(void);
    void flashingPattern3(void);
    void flashingPattern4(void);
    void ALSButton_isr(void);
    
    
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* als_h */
