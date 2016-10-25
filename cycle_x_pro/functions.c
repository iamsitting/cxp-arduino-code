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

void getTime(){
    uint16_t elapsedMillis = millis() - g_wOffsetTime;
    uint16_t elapsedTime = (elapsedMillis)/1000;
    g_TimeStamp.hour = numberOfHours(elapsedTime) & 0x00FF;
    g_TimeStamp.minute = numberOfMinutes(elapsedTime) & 0x00FF;
    g_TimeStamp.second.bits32 = numberOfSeconds(elapsedTime) + (float32_t)(elapsedMillis % 1000)/1000;
}

void updateData(){
    
    switch(g_byNextUpdate){
        case 0:
            updateBatteryLevel();
            g_byNextUpdate++;
            break;
        case 1:
            updateSpeed();
            g_byNextUpdate++;
            break;
        case 2:
            updateDistance();
            g_byNextUpdate++;
            break;
        case 3:
            updateCalories();
            g_byNextUpdate++;
            break;
        case 4:
            updateADS();
        default:
            g_byNextUpdate = 0;
            
    }
}
void updateBatteryLevel(){
    //Battery from Analog pin
    g_byBatteryLevel = 90;
}
void updateADS(){
    //Accident detection code here
    g_byThreat = 0;
}
void updateSpeed(){
    //Speed alg.
    g_fSpeed.bits32 = (float) analogRead(POT_PIN) * 9.77517;
    
}
void updateDistance(){
    //Distance alg.
    g_fDistance.bits32 = g_fSpeed.bits32 + 1;
}
void updateCalories(){
    //Calories alg.
    g_fCalories.bits32 = g_fDistance.bits32 + 1;
}
void getERPS(){
    //get location data
    g_fLatitude.bits32 = (float) 30.6209756;
    g_fLongitude.bits32 = (float) -96.3395046;
}
void getRaceData(){
    //get race data
    g_fOppSpeed.bits32 = g_fCalories.bits32 + 1;
    g_fOppDistance.bits32 = g_fOppSpeed.bits32 + 1;
}
void byteWrite(uint8_t protocol){
    uint8_t packet[BUFFER_SIZE];
    uint8_t i = 0;
    uint8_t c = 0;
    uint16_t checksum = 0;
    
    switch(protocol){
        case SEND_IDLE:
            packet[0] = g_byBatteryLevel;
            packet[1] = g_byThreat;
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
            
            packet[0] = g_byBatteryLevel;
            packet[1] = g_byThreat;
            
            //time
            packet[2] = g_TimeStamp.hour;
            packet[3] = g_TimeStamp.minute;
            packet[4] = g_TimeStamp.second.by.te3;
            packet[5] = g_TimeStamp.second.by.te2;
            packet[6] = g_TimeStamp.second.by.te1;
            packet[7] = g_TimeStamp.second.by.te0;
            
            //speed
            packet[8] = g_fSpeed.by.te3;
            packet[9] = g_fSpeed.by.te2;
            packet[10] = g_fSpeed.by.te1;
            packet[11] = g_fSpeed.by.te3;
            
            //distance
            packet[12] = g_fDistance.by.te3;
            packet[13] = g_fDistance.by.te2;
            packet[14] = g_fDistance.by.te1;
            packet[15] = g_fDistance.by.te0;
            
            //calories
            packet[16] = g_fCalories.by.te3;
            packet[17] = g_fCalories.by.te2;
            packet[18] = g_fCalories.by.te1;
            packet[19] = g_fCalories.by.te0;
            
            
            for(c = 0; c<20; c++){
                checksum += packet[c];
            }
            
            packet[20] = checksum & 0xFF;
            packet[21] = protocol;
            packet[22] = 0xA7;
            
            break;
        case SEND_HEADER:
            
            packet[0] = 0x74; //t
            packet[1] = 0x69; //i
            packet[2] = 0x6D; //m
            packet[3] = 0x65; //e
            packet[4] = 0x2C; //,
            
            packet[5] = 0x73; //s
            packet[6] = 0x70; //p
            packet[7] = 0x65; //e
            packet[8] = 0x65; //e
            packet[9] = 0x64; //d
            packet[10] = 0x2C; //,
            
            packet[11] = 0x64; //d
            packet[12] = 0x69; //i
            packet[13] = 0x73; //s
            packet[14] = 0x74; //t
            packet[15] = 0x61; //a
            packet[16] = 0x6E; //n
            packet[17] = 0x63; //c
            packet[18] = 0x65; //e
            packet[19] = 0x2C; //,
            
            packet[20] = 0x63; //c
            packet[21] = 0x61; //a
            packet[22] = 0x6C; //l
            packet[23] = 0x6F; //o
            packet[24] = 0x72; //r
            packet[25] = 0x69; //i
            packet[26] = 0x65; //e
            packet[27] = 0x73; //s
            
            for(c = 0; c<27; c++){
                checksum += packet[c];
            }
            packet[28] = checksum & 0xFF;
            
            packet[29] = protocol;
            packet[30] = 0xA7;
            
            break;
        case SEND_ERPS:
            getERPS();
            
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
        case SEND_RACE:
            getTime();
            getRaceData();
            
            packet[0] = g_byBatteryLevel;
            packet[1] = g_byThreat;
            
            //time
            packet[2] = g_TimeStamp.hour;
            packet[3] = g_TimeStamp.minute;
            packet[4] = g_TimeStamp.second.by.te3;
            packet[5] = g_TimeStamp.second.by.te2;
            packet[6] = g_TimeStamp.second.by.te1;
            packet[7] = g_TimeStamp.second.by.te0;
            
            //speed
            packet[8] = g_fSpeed.by.te3;
            packet[9] = g_fSpeed.by.te2;
            packet[10] = g_fSpeed.by.te1;
            packet[11] = g_fSpeed.by.te3;
            
            //distance
            packet[12] = g_fDistance.by.te3;
            packet[13] = g_fDistance.by.te2;
            packet[14] = g_fDistance.by.te1;
            packet[15] = g_fDistance.by.te0;
            
            //calories
            packet[16] = g_fCalories.by.te3;
            packet[17] = g_fCalories.by.te2;
            packet[18] = g_fCalories.by.te1;
            packet[19] = g_fCalories.by.te0;
            
            //opponent-speed
            packet[20] = g_fOppSpeed.by.te3;
            packet[21] = g_fOppSpeed.by.te2;
            packet[22] = g_fOppSpeed.by.te1;
            packet[23] = g_fOppSpeed.by.te0;
            
            //opponent-distance
            packet[24] = g_fOppDistance.by.te3;
            packet[25] = g_fOppDistance.by.te2;
            packet[26] = g_fOppDistance.by.te1;
            packet[27] = g_fOppDistance.by.te0;
            
            for(c = 0; c<28; c++)
            {
                checksum += packet[c];
            }
            packet[29] = checksum & 0xFF;
            packet[30] = protocol;
            packet[31] = 0xA7;
            break;
        default:
            break;
    }
    
    for(i = 0; i < BUFFER_SIZE; i++){
        g_bySendPacket[i] = packet[i];
    }
    
    g_byBTSendFlag = 1;
}
