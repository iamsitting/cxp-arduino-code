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
    g_fLatitude.bits32 = (float) 30.6209756;
    g_fLongitude.bits32 = (float) -96.3395046;
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
            
            //TODO: Handle this on the App
            packet[0] = g_byBatteryLevel;
            
            packet[1] = g_TimeStamp.hour;
            packet[2] = g_TimeStamp.minute;
            packet[3] = g_TimeStamp.second.by.te3;
            packet[4] = g_TimeStamp.second.by.te2;
            packet[5] = g_TimeStamp.second.by.te1;
            packet[6] = g_TimeStamp.second.by.te0;
            
            packet[7] = g_fMetric1.by.te3;
            packet[8] = g_fMetric1.by.te2;
            packet[9] = g_fMetric1.by.te1;
            packet[10] = g_fMetric1.by.te0;
            
            packet[11] = g_fMetric2.by.te3;
            packet[12] = g_fMetric2.by.te2;
            packet[13] = g_fMetric2.by.te1;
            packet[14] = g_fMetric2.by.te0;
            
            packet[15] = g_fMetric3.by.te3;
            packet[16] = g_fMetric3.by.te2;
            packet[17] = g_fMetric3.by.te1;
            packet[18] = g_fMetric3.by.te0;
            
            
            for(c = 0; c<19; c++){
                checksum += packet[c];
            }
            
            packet[19] = checksum & 0xFF;
            packet[20] = protocol;
            packet[21] = 0xA7;
            
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
    
    g_byBTSendFlag = 1;
}
