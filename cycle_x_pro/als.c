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
void ALSButton_isr(){
    if((millis() - g_wLastDebounceTime) > DEBOUNCE_DELAY){
        
        g_byFlashingPattern = (g_byFlashingPattern + 1 == 5 ? 0 : g_byFlashingPattern + 1);
        if(g_byFlashingPattern < 2) g_byChangedToSimple = 0;
        
#ifdef TEST_ERPS
        g_byMode = MODE_ERPS;
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
                break;
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