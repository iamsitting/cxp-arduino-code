//
//  functions.c
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/22/16.
//  Copyright © 2016 team14. All rights reserved.
//
#include <Arduino.h>
#include "globals.h"
#include "functions.h"
#include "testing.h"
//We will write data processing functions here
void test_getData(){
    getTime();
}

void test_getERPS(){
    g_fLatitude.bits32 = (float) 40;
    g_fLongitude.bits32 = (float) 0;
}

void getTime(){
    uint16_t elapsedMillis = millis() - g_wOffsetTime;
    uint16_t elapsedTime = (elapsedMillis)/1000;
    g_TimeStamp.hour = numberOfHours(elapsedTime) & 0x00FF;
    g_TimeStamp.minute = numberOfMinutes(elapsedTime) & 0x00FF;
    g_TimeStamp.second.bits32 = numberOfSeconds(elapsedTime) + (float32_t)(elapsedMillis % 1000)/1000;
}

void test_updateData(){
    
    switch(g_byNextUpdate){
        case 0:
            updateBatteryLevel();
            g_fMetric1.bits32 = (float) analogRead(POT_PIN) * 9.77517;
            g_byNextUpdate++;
            break;
        case 1:
            updateSpeed();
            g_fMetric2.bits32 = g_fMetric1.bits32 + 1;
            g_byNextUpdate++;
            break;
        case 2:
            updateADS();
            g_fMetric3.bits32 = g_fMetric2.bits32 + 1;
            g_byNextUpdate = 0;
            break;
        default:
            break;
            
    }
}
void updateADS(){
    //Accident detection code here
}
void updateSpeed(){
    //Speed from GPS/Acc. Here
}
void updateBatteryLevel(){
    g_byBatteryLevel = 90;
}

/********************* ALS Begin - Tyle Henderson *************************/

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

/********************* ALS End - Tyler Henderson *************************/
void byteWrite(uint8_t protocol){
    uint8_t packet[BUFFER_SIZE];
    uint8_t i = 0;
    uint8_t c = 0;
    uint16_t checksum = 0;
    
    switch(protocol){
        case SEND_BATTERY:
            packet[0] = g_byBatteryLevel;
            packet[1] = 1;
            packet[2] = 2;
            packet[3] = 3;
            packet[4] = 4;
            packet[5] = 5;
            packet[6] = 6;
            
            for(c = 0; c < 7; c++){
                checksum += packet[c];
            }
            
            packet[7] = checksum & 0xFF;
            packet[8] = protocol;
            packet[9] = 0xA7;
            
            break;
        case SEND_DATA:
            getTime();
            
            packet[0] = g_TimeStamp.hour;
            packet[1] = g_TimeStamp.minute;
            packet[2] = g_TimeStamp.second.by.te3;
            packet[3] = g_TimeStamp.second.by.te2;
            packet[4] = g_TimeStamp.second.by.te1;
            packet[5] = g_TimeStamp.second.by.te0;
            
            packet[6] = g_fMetric1.by.te3;
            packet[7] = g_fMetric1.by.te2;
            packet[8] = g_fMetric1.by.te1;
            packet[9] = g_fMetric1.by.te0;
            
            packet[10] = g_fMetric2.by.te3;
            packet[11] = g_fMetric2.by.te2;
            packet[12] = g_fMetric2.by.te1;
            packet[13] = g_fMetric2.by.te0;
            
            packet[14] = g_fMetric3.by.te3;
            packet[15] = g_fMetric3.by.te2;
            packet[16] = g_fMetric3.by.te1;
            packet[17] = g_fMetric3.by.te0;
            
            for(c = 0; c<18; c++){
                checksum += packet[c];
            }
            
            packet[18] = checksum & 0xFF;
            packet[19] = protocol;
            packet[20] = 0xA7;
            
            break;
        case SEND_HEADER:
            
            packet[0] = 0x74; //t
            packet[1] = 0x69; //i
            packet[2] = 0x6D; //m
            packet[3] = 0x65; //e
            packet[4] = 0x2C; //,
            
            packet[5] = 0x6D; //m
            packet[6] = 0x65; //e
            packet[7] = 0x74; //t
            packet[8] = 0x31; //1
            packet[9] = 0x2C; //,
            
            packet[10] = 0x6D; //m
            packet[11] = 0x65; //e
            packet[12] = 0x74; //t
            packet[13] = 0x32; //2
            packet[14] = 0x2C; //,
            packet[15] = 0x6D; //m
            packet[16] = 0x65; //e
            packet[17] = 0x74; //t
            packet[18] = 0x33; //3
            
            for(c = 0; c<19; c++){
                checksum += packet[c];
            }
            packet[19] = checksum & 0xFF;
            
            packet[20] = protocol;
            packet[21] = 0xA7;
            
            break;
        case SEND_ERPS:
            test_getERPS();
            
            packet[0] = g_fLatitude.by.te3;
            packet[1] = g_fLatitude.by.te2;
            packet[2] = g_fLatitude.by.te1;
            packet[3] = g_fLatitude.by.te0;
            
            packet[4] = g_fLongitude.by.te3;
            packet[5] = g_fLongitude.by.te2;
            packet[6] = g_fLongitude.by.te1;
            packet[7] = g_fLongitude.by.te0;
            
            for(c = 0; c<8; c++)
            {
                checksum += packet[c];
            }
            packet[8] = checksum & 0xFF;
            packet[9] = protocol;
            packet[10] = 0xA7;
            break;
        default:
            break;
    }
    
    for(i = 0; i < BUFFER_SIZE; i++){
        g_bySendPacket[i] = packet[i];
    }
}
