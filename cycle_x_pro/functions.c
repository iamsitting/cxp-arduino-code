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

float32_t generateData(uint8_t index){
    return sine_test[index];
}
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
    g_fSpeed.bits32 = generateData(ind);
    g_fOppSpeed.bits32 = generateData((ind+16) % 32);
    ind = (ind + 1 >= 32 ? 0: ind + 1);
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
    //g_fOppSpeed.bits32 = generateOppData();
    //g_fOppDistance.bits32 = g_fOppSpeed.bits32 + 1;
}
void byteWrite(uint8_t protocol){
    uint8_t packet[BUFFER_SIZE];
    uint8_t i = 0;
    uint8_t c = 0;
    uint16_t checksum = 0;
    
    switch(protocol){
        case SEND_IDLE:
            packet[i++] = 0xA7;
            packet[i++] = protocol;
            packet[i++] = g_byBatteryLevel;
            packet[i++]= g_byThreat;
            packet[i++] = 2;
            packet[i++] = 3;
            packet[i++] = 4;
            packet[i++] = 5;
            packet[i++] = 6;
            
            for(c = 0; c < i; c++){
                checksum += packet[c];
            }
            
            packet[i] = checksum & 0xFF;
            //10
            break;
        case SEND_DATA:
            getTime();
            
            packet[i++] = 0xA7;
            packet[i++] = protocol;
            
            packet[i++] = g_byBatteryLevel;
            packet[i++] = g_byThreat;
            
            //time
            packet[i++] = g_TimeStamp.hour;
            packet[i++] = g_TimeStamp.minute;
            packet[i++] = g_TimeStamp.second.by.te3;
            packet[i++] = g_TimeStamp.second.by.te2;
            packet[i++] = g_TimeStamp.second.by.te1;
            packet[i++] = g_TimeStamp.second.by.te0;
            
            //speed
            packet[i++] = g_fSpeed.by.te3;
            packet[i++] = g_fSpeed.by.te2;
            packet[i++] = g_fSpeed.by.te1;
            packet[i++] = g_fSpeed.by.te0;
            
            //distance
            packet[i++] = g_fDistance.by.te3;
            packet[i++] = g_fDistance.by.te2;
            packet[i++] = g_fDistance.by.te1;
            packet[i++] = g_fDistance.by.te0;
            
            //calories
            packet[i++] = g_fCalories.by.te3;
            packet[i++] = g_fCalories.by.te2;
            packet[i++] = g_fCalories.by.te1;
            packet[i++] = g_fCalories.by.te0;
            
            
            for(c = 0; c<i; c++){
                checksum += packet[c];
            }
            
            packet[i] = checksum & 0xFF;
            //23
            
            break;
        case SEND_HEADER:
            
            packet[i++] = 0xA7;
            packet[i++] = protocol;
            
            packet[i++] = 0x74; //t
            packet[i++] = 0x69; //i
            packet[i++] = 0x6D; //m
            packet[i++] = 0x65; //e
            packet[i++] = 0x2C; //,
            
            packet[i++] = 0x73; //s
            packet[i++] = 0x70; //p
            packet[i++] = 0x65; //e
            packet[i++] = 0x65; //e
            packet[i++] = 0x64; //d
            packet[i++] = 0x2C; //,
            
            packet[i++] = 0x64; //d
            packet[i++] = 0x69; //i
            packet[i++] = 0x73; //s
            packet[i++] = 0x74; //t
            packet[i++] = 0x61; //a
            packet[i++] = 0x6E; //n
            packet[i++] = 0x63; //c
            packet[i++] = 0x65; //e
            packet[i++] = 0x2C; //,
            
            packet[i++] = 0x63; //c
            packet[i++] = 0x61; //a
            packet[i++] = 0x6C; //l
            packet[i++] = 0x6F; //o
            packet[i++] = 0x72; //r
            packet[i++] = 0x69; //i
            packet[i++] = 0x65; //e
            packet[i++] = 0x73; //s
            
            for(c = 0; c<i; c++){
                checksum += packet[c];
            }
            packet[i] = checksum & 0xFF;
            //31
            
            
            break;
        case SEND_ERPS:
            getERPS();
            
            packet[i++] = 0xA7;
            packet[i++] = protocol;
            
            packet[i++] = g_fLatitude.by.te3;
            packet[i++] = g_fLatitude.by.te2;
            packet[i++] = g_fLatitude.by.te1;
            packet[i++] = g_fLatitude.by.te0;
            
            packet[i++] = g_fLongitude.by.te3;
            packet[i++] = g_fLongitude.by.te2;
            packet[i++] = g_fLongitude.by.te1;
            packet[i++] = g_fLongitude.by.te0;
            
            for(c = 0; c<i; c++)
            {
                checksum += packet[c];
            }
            packet[i] = checksum & 0xFF;
            //11
            break;
        case SEND_RACE:
            getTime();
            //getRaceData();
            
            packet[i++] = 0xA7;
            packet[i++] = protocol;
            
            packet[i++] = g_byBatteryLevel;
            packet[i++] = g_byThreat;
            
            //time
            packet[i++] = g_TimeStamp.hour;
            packet[i++] = g_TimeStamp.minute;
            packet[i++] = g_TimeStamp.second.by.te3;
            packet[i++] = g_TimeStamp.second.by.te2;
            packet[i++] = g_TimeStamp.second.by.te1;
            packet[i++] = g_TimeStamp.second.by.te0;
            
            //speed
            packet[i++] = g_fSpeed.by.te3;
            packet[i++] = g_fSpeed.by.te2;
            packet[i++] = g_fSpeed.by.te1;
            packet[i++] = g_fSpeed.by.te3;
            
            //distance
            packet[i++] = g_fDistance.by.te3;
            packet[i++] = g_fDistance.by.te2;
            packet[i++] = g_fDistance.by.te1;
            packet[i++] = g_fDistance.by.te0;
            
            //calories
            packet[i++] = g_fCalories.by.te3;
            packet[i++] = g_fCalories.by.te2;
            packet[i++] = g_fCalories.by.te1;
            packet[i++] = g_fCalories.by.te0;
            
            //opponent-speed
            packet[i++] = g_fOppSpeed.by.te3;
            packet[i++] = g_fOppSpeed.by.te2;
            packet[i++] = g_fOppSpeed.by.te1;
            packet[i++] = g_fOppSpeed.by.te0;
            
            //opponent-distance
            packet[i++] = g_fOppDistance.by.te3;
            packet[i++] = g_fOppDistance.by.te2;
            packet[i++] = g_fOppDistance.by.te1;
            packet[i++] = g_fOppDistance.by.te0;
            
            for(c = 0; c<i; c++)
            {
                checksum += packet[c];
            }
            packet[i] = checksum & 0xFF;
            //31
            break;
        default:
            break;
    }
    
    for(i = 0; i < BUFFER_SIZE; i++){
        g_bySendPacket[i] = packet[i];
    }
    
    g_byBTSendFlag = 1;
}

void btParse(){

    switch (g_byRecvPacket[0]) {
            //Apply correct Mode of operation and System status
        case 0x43: //C - BT Connected
        case 0x63:
            SET_STATUS(g_byStatus, RTS);
            SET_STATUS(g_byStatus, BTCON); //clear on app/BT destroy
            break;
        case 0x45: //E - ERPS ACK
        case 0x65:
            SET_STATUS(g_byStatus, ERPS);
            break;
        case 0x4B: //K - Send next sample
        case 0x6B:
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x4E: //N - Retry New Session
        case 0x6E:
            SET_STATUS(g_byStatus, RTS);
            SET_STATUS(g_byStatus, NEW_SESSION);
            break;
        case 0x51: //Q - End Session
        case 0x71:
            g_byMode = MODE_IDLE;
            CLEAR_STATUS(g_byStatus, NEW_SESSION);
            CLEAR_STATUS(g_byStatus, ERPS);
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x52: //R - Reset ERPS
        case 0x72:
            g_byMode = MODE_IDLE;
            CLEAR_STATUS(g_byStatus, ERPS);
            break;
        case 0x57: //W - New Solo Session
        case 0x77:
            g_byMode = MODE_SOLO;
            SET_STATUS(g_byStatus, NEW_SESSION);
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x58: //X - New Trainee Session
        case 0x78:
            g_byMode = MODE_TRAINEE;
            SET_STATUS(g_byStatus, NEW_SESSION);
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x59: //Y - New Trainer Session
        case 0x79:
            g_byMode = MODE_TRAINER;
            SET_STATUS(g_byStatus, NEW_SESSION);
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x5A: //Z - New Race Session
        case 0x7A:
            g_byMode = MODE_RACE;
            SET_STATUS(g_byStatus, NEW_SESSION);
            SET_STATUS(g_byStatus, RTS);
            break;
        default:
            //do nothing
            __asm__("nop\n\t");
    }
}
