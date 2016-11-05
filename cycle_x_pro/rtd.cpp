//
//  rtd.cpp
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 11/2/16.
//  Copyright © 2016 team14. All rights reserved.
//

#include <Arduino.h>
#include "rtd.h"
#include "globals.h"
#include "types.h"
/********************* Written by Alejandro Penaloza *************************/

void getLocation(){
    g_fLongitude.bits32 = gps.location.lng();
    g_fLatitude.bits32 = gps.location.lat();
    
}

void getSpeed(){

    if (gps.speed.isUpdated()){
        g_fSpeed.bits32 = gps.speed.mph();
    } else {
        g_fSpeed.bits32 = calculateSpeed(g_fSpeed.bits32);
    }
}

void getDistance(){
    
    float dt;
    unsigned long currentMillis = millis(); //Internal counter
    dt = (currentMillis - previousMillis_distance) / ((float)1000);
    previousMillis_distance = currentMillis;
    
    if(g_fSpeed.bits32 > MIN_SPEED){
        //TODO: Check float/double type issue
        g_fDistance.bits32 = g_fDistance.bits32 + (g_fSpeed.bits32 / 3600) * dt;
    }
}

void getCalories() {
    
    float32_t cal_sec = 0;
    float32_t dt;
    unsigned long currentMillis = millis(); //Internal counter
    dt = (currentMillis - previousMillis_cal) / ((double)1000);
    previousMillis_cal = currentMillis; // Current Millis from above may be different than this current millis.
    
    
    if(g_fSpeed.bits32 < MIN_SPEED){
        cal_sec = 0;
    }
    else if(g_fSpeed.bits32 < 10){
        cal_sec = (((0.0301) * (g_halfWeight)) + 0.037) / 60 * dt;
    }
    else if(g_fSpeed.bits32 < 12){
        cal_sec = (((0.0455) * (g_halfWeight)) - 0.0331) / 60 * dt;
    }
    else if(g_fSpeed.bits32 < 14){
        cal_sec = (((0.0605) * (g_halfWeight)) - 0.0085) / 60 * dt;
    }
    else if(g_fSpeed.bits32 < 16){
        cal_sec = (((0.0756) * (g_halfWeight)) + 0.009) / 60 * dt;
    }
    else if(g_fSpeed.bits32 < 20){
        cal_sec = (((0.0908) * (g_halfWeight)) + 0.0275) / 60 * dt;
    }
    else if(g_fSpeed.bits32 >= 20){
        cal_sec = (((0.121) * (g_halfWeight)) - 0.0106) / 60 * dt;
    }
    
    g_fCalories.bits32 = g_fCalories.bits32 + cal_sec;

    
}

void getADS(){
    
    float32_t At, Gt;
    uint8_t possibleAccident = 0;
    
    At = getAcceleration(TOTAL_SUM);
    Gt = getGyro(TOTAL_SUM);
    
    if ( (At > 4 && Gt > 250) || (At > 6) || (Gt > 320) )
    {
        SET_STATUS(g_byStatus, POSS_ACC);
        previousMillis_fad = millis();
    } else {
        CLEAR_STATUS(g_byStatus, POSS_ACC);
    }
}

void checkFalseAlarm (){
  
  unsigned long currentMillis = millis(); //Internal counter

  if (currentMillis - previousMillis_fad >= FALSE_ALARM_WINDOW){

    if(g_fSpeed.bits32 < MIN_SPEED){

        SET_STATUS(g_byStatus, ERPS);
        CLEAR_STATUS(g_byStatus, POSS_ACC);
    } else {
        CLEAR_STATUS(g_byStatus, POSS_ACC);
    }
    
   
 }     
}

float32_t calculateSpeed(float32_t initSpeed){
    
    float32_t dt, Ay, finalSpeed;
    unsigned long currentMillis = millis(); //Internal counter  

    Ay = getAcceleration(Y_DIRECTION);
    dt = (currentMillis - previousMillis_dre) / ((double) 1000);
    previousMillis_dre = currentMillis; // Current Millis from above may be different than this current millis.
    finalSpeed = abs ((initSpeed) + (((Ay) * (9.8*1609.344)/3600) * dt));

    return finalSpeed;

}

float32_t convertRawGyro(int16_t gRaw) {
    // since we are using 250 degrees/seconds range
    // -250 maps to a raw value of -32768
    // +250 maps to a raw value of 32767
    
    float32_t g = (gRaw * 250.0) / 32768.0;
    
    return g;
}

float32_t convertRawAcceleration(int16_t aRaw) {
    // since we are using 16G range
    // -16g maps to a raw value of -32768
    // +16g maps to a raw value of 32767
    
    float32_t a = (aRaw * 16.0) / 32768.0;
    
    return a;
}

float32_t getGyro(uint8_t dir){
    
    int gxRaw, gyRaw, gzRaw;         // raw gyro values
    float32_t Gx, Gy, Gz, Gt;
    
    Bmi.readGyro(gxRaw, gyRaw, gzRaw);
    
    // convert the raw gyro data to degrees/second
    Gx = convertRawGyro(gxRaw);
    Gy = convertRawGyro(gyRaw);
    Gz = convertRawGyro(gzRaw);
    
    //Linear Combination of Gyroscope data.
    Gt = abs(Gx) + abs (Gy) + abs(Gz);

    switch(dir){
      case X_DIRECTION:
        return Gx;
        break;
      case Y_DIRECTION:
        return Gy;
        break;
      case Z_DIRECTION:
        return Gz;
        break;
      case TOTAL_SUM:
      default:
        return Gt;
    }
    
    
}

float32_t getAcceleration(uint8_t dir){
    int axRaw, ayRaw, azRaw;         // raw acc. values
    float32_t Ax, Ay, Az, At;
    
    Bmi.readAccelerometer(axRaw, ayRaw, azRaw);
    
    // convert the raw accelerometer data to G's
    Ax = convertRawAcceleration(axRaw);
    Ay = convertRawAcceleration(ayRaw);
    Az = convertRawAcceleration(azRaw);
    
    //At = abs(Ax) + abs(Ay) + abs(Az);
    At = sqrt(pow(Ax, 2) + pow(Ay, 2) + pow(Az, 2));

    switch(dir){
      case X_DIRECTION:
        return Ax;
        break;
      case Y_DIRECTION:
        return Ay;
        break;
      case Z_DIRECTION:
        return Az;
        break;
      case TOTAL_SUM:
      default:
        return At;
    }
}

void setupRTD(){
    GP20U7.begin(GPS_BAUD);
    // initialize device
    Bmi.begin(BMI_PIN);
    Bmi.setAccelerometerRange(16);
    
}
/********************* Written by Alejandro Penaloza *************************/
