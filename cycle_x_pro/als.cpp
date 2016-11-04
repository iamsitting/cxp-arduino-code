//
//  als.cpp
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/29/16.
//  Copyright © 2016 team14. All rights reserved.
//

#include <Arduino.h>
#include "globals.h"
#include "als.h"
#include "rtd.h"

/********************* Written by Tyler Henderson *************************/
void ALSButton_isr(){
    if((millis() - g_wLastDebounceTime) > DEBOUNCE_DELAY){
        
        g_byFlashingPattern = (g_byFlashingPattern + 1 >= 5 ? 0 : g_byFlashingPattern + 1);
        if(g_byFlashingPattern < 2) g_byChangedToSimple = 0;
        
#ifdef TEST_ERPS
        SET_STATUS(g_byStatus, ERPS);
#endif //TEST_ERPS
        
        g_wLastDebounceTime = millis();
    }
}

void switchFlashingPattern(){
    switch (g_byFlashingPattern) {
        case 0:
            
            if(g_byChangedToSimple == 0){
                flashingPattern0(); //simple
                g_byChangedToSimple = 1;
            }
            break;
        case 1:
            
            if(g_byChangedToSimple == 0){
                flashingPattern1(); //simple
                g_byChangedToSimple = 1;
            }
            break;
        case 2:
            flashingPattern2(); //complex
            break;
        case 3:
            flashingPattern3(); //complex
            break;
        case 4:
            flashingPattern4(); //complex
            break;
        default:
            g_byFlashingPattern = 0;
            break;
    }
}

void flashingPattern0(){
    digitalWrite(ALSPIN1, LOW);
    digitalWrite(ALSPIN2, LOW);
    digitalWrite(ALSPIN3, LOW);
}

void flashingPattern1(){
    digitalWrite(ALSPIN1, HIGH);
    digitalWrite(ALSPIN2, HIGH);
    digitalWrite(ALSPIN3, HIGH);
}

void flashingPattern2(){
    uint16_t currentMillis = millis();
    if(currentMillis - g_wPreviousMillis >= FLASH_INTERVAL) {
        g_wPreviousMillis = currentMillis;
        
        g_byALSPin1State = ~g_byALSPin1State;
        g_byALSPin2State = ~g_byALSPin2State;
        g_byALSPin3State = ~g_byALSPin3State;
        
        digitalWrite(ALSPIN1, g_byALSPin1State);
        digitalWrite(ALSPIN2, g_byALSPin2State);
        digitalWrite(ALSPIN3, g_byALSPin3State);
    }
}

void flashingPattern3(){
    uint16_t currentMillis = millis();
    if(currentMillis - g_wPreviousMillis >= FLASH_INTERVAL) {
        g_wPreviousMillis = currentMillis;
        
        switch (g_byFlashingCount) {
            case 1:
                digitalWrite(ALSPIN1, HIGH);
                digitalWrite(ALSPIN2, LOW);
                digitalWrite(ALSPIN3, LOW);
                break;
            case 2:
                digitalWrite(ALSPIN1, LOW);
                digitalWrite(ALSPIN2, HIGH);
                digitalWrite(ALSPIN3, LOW);
                break;
            case 3:
                digitalWrite(ALSPIN1, HIGH);
                digitalWrite(ALSPIN2, HIGH);
                digitalWrite(ALSPIN3, LOW);
                break;
            case 4:
                digitalWrite(ALSPIN1, LOW);
                digitalWrite(ALSPIN2, LOW);
                digitalWrite(ALSPIN3, HIGH);
                break;
            default:
                digitalWrite(ALSPIN1, LOW);
                digitalWrite(ALSPIN2, LOW);
                digitalWrite(ALSPIN3, LOW);
                break;
        }
        
        g_byFlashingCount = (g_byFlashingCount + 1 >= 5 ? 1 : g_byFlashingCount + 1);
    }
}

void flashingPattern4(){
    uint16_t currentMillis = millis();
    if(currentMillis - g_wPreviousMillis >= FLASH_INTERVAL) {
        g_wPreviousMillis = currentMillis;
        
        switch (g_byFlashingCount) {
            case 1:
            case 3:
                digitalWrite(ALSPIN1, HIGH);
                digitalWrite(ALSPIN2, LOW);
                digitalWrite(ALSPIN3, HIGH);
                break;
            case 5:
            case 7:
                digitalWrite(ALSPIN1, LOW);
                digitalWrite(ALSPIN2, HIGH);
                digitalWrite(ALSPIN3, HIGH);
                break;
            default:
                digitalWrite(ALSPIN1, LOW);
                digitalWrite(ALSPIN2, LOW);
                digitalWrite(ALSPIN3, LOW);
                break;
        }
        
        g_byFlashingCount = (g_byFlashingCount + 1 >= 9 ? 1 : g_byFlashingCount + 1);
    }
}

void getUSThreat(){
  //Calculate distance from Ultrasonic Sensor
  int Volt_num;
  float32_t Vm, d;
  Volt_num= analogRead(USOUND_IN);
  Vm = .00080566 * Volt_num; //.00080566 (12bit at 3.3V)//.003223 (10bit at 3.3V)//.004883 (10bit at 5V)
  d = Vm / 0.006445 / 12; // [ft]
  if(d < US_MIN_DISTANCE){
    g_byThreat = 1;
  } else {
    g_byThreat = 0;
  }
}

void flashRearLEDS() {

    if(g_byThreat){
        unsigned long currentMillis = millis();

        if (currentMillis - g_wPreviousMillis2 >= REAR_FLASH_INTERVAL) {

            g_wPreviousMillis2 = currentMillis;
            g_byUsoundLtPinState = ~ g_byUsoundLtPinState;
            digitalWrite(USOUND_LT, g_byUsoundLtPinState);
        }
    } else {
        if(!g_byUsoundLtPinState){
            g_byUsoundLtPinState = LOW;
            digitalWrite(USOUND_LT, g_byUsoundLtPinState);
        }
    }
}

void changeBrakeLight(){
    float32_t Ax;

    Ax = getAcceleration(X_DIRECTION);
    if (Ax < 0) {
        g_byBrakeCounter++;

        if (g_byBrakeCounter >=4){
            if(!g_byBrakeLtPinState){
                g_byBrakeLtPinState = HIGH;
                digitalWrite(BRAKE_LT, g_byBrakeLtPinState);
            }
        }
    }
    else {
        if(!g_byBrakeLtPinState){
            g_byBrakeCounter = 0;
            g_byBrakeLtPinState = LOW;
            digitalWrite(BRAKE_LT, g_byBrakeLtPinState);
        }
    }
}

void setupALS(){
    pinMode(ALSPIN1, OUTPUT);
    pinMode(ALSPIN2, OUTPUT);
    pinMode(ALSPIN3, OUTPUT);
    pinMode(USOUND_LT, OUTPUT);
    pinMode(BRAKE_LT, OUTPUT);
    pinMode(USOUND_IN, INPUT);
    analogReadResolution(12);
    pinMode(ALS_BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(ALS_BUTTON_PIN), ALSButton_isr, RISING);
}
/********************* Written by Tyler Henderson *************************/