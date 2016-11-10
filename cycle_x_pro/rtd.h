//
//  rtd.h
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 11/2/16.
//  Copyright © 2016 team14. All rights reserved.
//

#ifndef rtd_h
#define rtd_h

#include "globals.h"
#include "types.h"
#include <TinyGPS++.h>
#include <BMI160Gen.h>
    
#define GPS_BAUD 9600
#define BMI_PIN 52

#define MIN_SPEED 0.5
#define FALSE_ALARM_WINDOW 500
#define X_DIRECTION 1
#define Y_DIRECTION 2
#define Z_DIRECTION 3
#define TOTAL_SUM   0

extern union Floater32_t g_fSpeed;
extern union Floater32_t g_fDistance;
extern union Floater32_t g_fCalories;
extern union Floater32_t g_fLatitude;
extern union Floater32_t g_fLongitude;
extern uint32_t previousMillis_distance;
extern uint32_t previousMillis_cal;
extern uint32_t previousMillis_dre;
extern uint32_t previousMillis_fad;
extern uint16_t g_halfWeight;
extern uint8_t g_byGPSMisses;

extern TinyGPSPlus gps;
extern BMI160GenClass Bmi;

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus    


    /** RTD & ADS Functions **/
    void getLocation(void);
    void getSpeed(void);
    void getDistance(void);
    void getCalories(void);
    float32_t convertRawGyro(int16_t gRaw);
    float32_t convertRawAcceleration(int16_t aRaw);
    float32_t getGyro(uint8_t dir);
    float32_t getAcceleration(uint8_t dir);
    void getADS(void);
    float32_t calculateSpeed(float32_t initSpeed);
    void checkFalseAlarm(void);
    void setupRTD(void);
    
    
#ifdef __cplusplus
}
#endif //__cplusplus


#endif /* rtd_h */
