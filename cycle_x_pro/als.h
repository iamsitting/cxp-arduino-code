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

#define DEBOUNCE_DELAY 300
#define FLASH_INTERVAL 100
#define REAR_FLASH_INTERVAL 100

#define US_MIN_DISTANCE 20

extern volatile uint8_t g_byChangedToSimple;
extern volatile uint32_t g_wLastDebounceTime;
extern volatile uint8_t g_byFlashingPattern;
extern uint32_t g_wPreviousMillis;
extern uint32_t g_wPreviousMillis2;
extern uint8_t g_byALSPin1State;
extern uint8_t g_byALSPin2State;
extern uint8_t g_byALSPin3State;
extern uint8_t g_byUsoundLtPinState;
extern uint8_t g_byFlashingCount;
extern uint8_t g_byBatteryLevel;
extern uint8_t g_byThreat;
extern uint8_t g_byBrakeCounter;
extern uint8_t g_byBrakeLtPinState;
extern uint8_t g_byRelayState;

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
    void flashRearLEDS(void);
    void ALSButton_isr(void);
    void getUSThreat(void);
    void setupALS(void);
    void changeBrakeLight(void);
    void getBatteryLevel(void);
    uint8_t roundTo5(float32_t x);
    void switchRelay(void);
    
    
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* als_h */
