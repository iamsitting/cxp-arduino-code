//
//  als.c
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/29/16.
//  Copyright © 2016 team14. All rights reserved.
//

#include <Arduino.h>
#include "globals.h"
#include "als.h"

/********************* Written by Tyler Henderson *************************/

void readDebounceButton(){
    uint8_t reading = digitalRead(BUTTON_PIN);
    
    if(reading != g_byLastButtonState){
        g_wLastDebounceTime = millis();
    }
    
    if((millis() - g_wLastDebounceTime) > DEBOUNCE_DELAY){
        if(reading != g_byCurrentButtonState){
            g_byCurrentButtonState = reading;
            
            if(g_byCurrentButtonState == HIGH){
                g_byFlashingPattern = (g_byFlashingPattern + 1 == 5 ? 0 : g_byFlashingPattern + 1);
#ifdef TEST_CODE
                g_byMode = MODE_ERPS;
#endif //TEST_CODE
            }
        }
    }
    g_byLastButtonState = reading;
}

void switchFlashingPattern(){
    switch (g_byFlashingPattern) {
        case 0:
            flashingPattern0();
            break;
        case 1:
            flashingPattern1();
            break;
        case 2:
            flashingPattern2();
            break;
        case 3:
            flashingPattern3();
            break;
        case 4:
            flashingPattern4();
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
            default:
                digitalWrite(ALSPIN1, LOW);
                digitalWrite(ALSPIN2, LOW);
                digitalWrite(ALSPIN3, LOW);
                break;
        }
        
        g_byFlashingCount = (g_byFlashingCount + 1 == 5 ? 1 : g_byFlashingCount + 1);
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
            default:
                digitalWrite(ALSPIN1, LOW);
                digitalWrite(ALSPIN2, LOW);
                digitalWrite(ALSPIN3, LOW);
                break;
        }
        
        g_byFlashingCount = (g_byFlashingCount + 1 == 9 ? 1 : g_byFlashingCount + 1);
    }
}

/********************* Written by Tyler Henderson *************************/