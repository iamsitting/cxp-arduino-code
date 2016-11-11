//
//  functions.cpp
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 9/22/16.
//  Copyright © 2016 team14. All rights reserved.
//
#include <Arduino.h>
#include "globals.h"
#include "functions.h"
#include "rtd.h"
#include "als.h"
#include "trio.h"

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

void updateBatteryLevel(){
    //Battery from Analog pin
    g_byBatteryLevel = 90;
}

void getRaceData(){
  g_fOppSpeed.bits32 = g_fSpeed.bits32 + 10;
    //get race data
    //g_fOppSpeed.bits32 = generateOppData();
    //g_fOppDistance.bits32 = g_fOppSpeed.bits32 + 1;
}

void updateData(){
    
    switch(g_byNextUpdate){
        case 0:
            getSpeed();
            switchRelay();
            getDistance();
            g_byNextUpdate++;
            break;
        case 1:
            //TODO: add to als
            getBatteryLevel();
            g_byNextUpdate++;
            break;
        case 2:
            getCalories();
            g_byNextUpdate++;
            break;
        case 3:
            getLocation();
            g_byNextUpdate++;
            break;
        case 4:
            getADS();
        default:
            g_byNextUpdate = 0;
            
    }
}

void updateData2(){
    
    switch(g_byNextUpdate){
        case 0:
        //getSpeed() getDistance()
            g_fSpeed.bits32 = generateData(ind++);
            //getSpeed();
            g_fDistance.bits32 = g_fSpeed.bits32 + 5;
            g_byNextUpdate++;
            break;
        case 1:
            //TODO: add to als
            //getBatteryLevel()
            g_byBatteryLevel = 90;
            g_byNextUpdate++;
            break;
        case 2:
        //getCalories()
            g_fCalories.bits32 = g_fSpeed.bits32 + 5;
            g_byNextUpdate++;
            break;
        case 3:
        //getThreat()
            g_byThreat = 0;
            g_byNextUpdate++;
            break;
        case 4:
            getLocation();
            g_byNextUpdate++;
            break;
        case 5:
            //getUSThreat();
            g_byNextUpdate++;
            break;
        case 6:
            getADS();
        default:
            g_byNextUpdate = 0;
            
    }
}

void BluetoothBuildMessage(uint8_t protocol){
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
            getRaceData();
            
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

void getNameWeight(){
    uint8_t i = 0;
    
    g_halfWeight = (g_byRecvPacket[1]-30)*100 +
        (g_byRecvPacket[2]-30)*10 +
        g_byRecvPacket[3]-30;
            
    for(i=0; i<NAME_SIZE; i++){
        if(g_byRecvPacket[i+4] != '\n'){
            g_byUserName[i] = g_byRecvPacket[i+4];
        } else{
            g_byUserName[i] = g_byRecvPacket[i+4];
            break;
        } 
    }            
}

void BluetoothDeconstructMessage(){

    switch (g_byRecvPacket[0]) {
            //Apply correct Mode of operation and System status
        case 0x43: //C - BT Connected
        case 0x63:
            getNameWeight();
            SET_STATUS(g_byStatus, RTS);
            SET_STATUS(g_byStatus, BTCON); //clear on app/BT destroy
            break;
        case 0x45: //E - ERPS ACK
        case 0x65:
            SET_STATUS(g_byStatus, ERPS_ACK);
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
            CLEAR_STATUS(g_byStatus, ERPS_ACK);
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x52: //R - Reset ERPS
        case 0x72:
            g_byMode = MODE_IDLE;
            CLEAR_STATUS(g_byStatus, ERPS);
            CLEAR_STATUS(g_byStatus, ERPS_ACK);
            break;
        case 0x57: //W - New Solo Session
        case 0x77:
            g_byMode = MODE_SOLO;
            SET_STATUS(g_byStatus, NEW_SESSION);
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x58: //X - New Athelete Session
        case 0x78:
            g_byMode = MODE_ATHLETE;
            SET_STATUS(g_byStatus, NEW_SESSION);
            SET_STATUS(g_byStatus, RTS);
            break;
        case 0x59: //Y - New Coach Session
        case 0x79:
            g_byMode = MODE_COACH;
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


/** Input and output functions **/
void BluetoothReceive() {
    uint8_t i = 0;
    uint8_t inChar = 0;
    while (HC06.available()){
        inChar = HC06.read();
        g_byRecvPacket[i++] = inChar;
        if(inChar == '\n'){
            g_byBTRecvFlag = 1;
        }
    }
}

void BluetoothSend() {
    //if ERPS
    if(CHECK_STATUS(g_byStatus, ERPS)){
        if(!CHECK_STATUS(g_byStatus, ERPS_ACK)){
          BluetoothBuildMessage(SEND_ERPS);
            
        } else{
            uint16_t currentMillis = millis();
            if(currentMillis - g_wIdleMillis > TEN_SECONDS) {
                g_wIdleMillis = currentMillis;
                BluetoothBuildMessage(SEND_IDLE);
                
            }
        }
    } else {
        //Else operate normally
        switch (g_byMode) {
                
                //This is a standby mode
            case MODE_IDLE:
            {
                if(CHECK_STATUS(g_byStatus, BTCON)){
                    uint16_t currentMillis = millis();
                    if(currentMillis - g_wIdleMillis > 500) {
                        g_wIdleMillis = currentMillis;
                        BluetoothBuildMessage(SEND_IDLE);
                    }
                }
            }
                
                
                //__asm__("nop\n\t");
                break;
                
                //RTD Modes
            case MODE_SOLO:
            case MODE_ATHLETE:
            case MODE_COACH:
            {
                uint16_t currentMillis = millis();
                if(currentMillis - g_wDataMillis > 100) {
                    g_wDataMillis = currentMillis;
                    if (CHECK_STATUS(g_byStatus, RTS)) {
                        
                        if (CHECK_STATUS(g_byStatus, NEW_SESSION)) {
                            g_wOffsetTime = millis();
                            BluetoothBuildMessage(SEND_HEADER);
                            CLEAR_STATUS(g_byStatus, NEW_SESSION);
                        } else {
                          BluetoothBuildMessage(SEND_DATA);
                        }
                        g_byMisses = 0;
                        CLEAR_STATUS(g_byStatus, RTS);
                    } else {
                        g_byMisses++;
                        if(g_byMisses > MISSES_ALLOWED){
                            SET_STATUS(g_byStatus, RTS);
                        }
                    }
                    
                }
                
            }
                break;
            case MODE_RACE:
            {
                uint16_t currentMillis = millis();
                if(currentMillis - g_wDataMillis > 100) {
                    g_wDataMillis = currentMillis;
                    if (CHECK_STATUS(g_byStatus, RTS)) {
                        
                        if (CHECK_STATUS(g_byStatus, NEW_SESSION)) {
                            g_wOffsetTime = millis();
                            BluetoothBuildMessage(SEND_HEADER);
                            CLEAR_STATUS(g_byStatus, NEW_SESSION);
                        } else {
                            BluetoothBuildMessage(SEND_RACE);
                        }
                        g_byMisses = 0;
                        CLEAR_STATUS(g_byStatus, RTS);
                    } else {
                        g_byMisses++;
                        if(g_byMisses > MISSES_ALLOWED){
                            SET_STATUS(g_byStatus, RTS);
                        }
                    }
                    
                }
                
            }
                break;
            default:
                __asm__("nop\n\t");
                break;
        }
    }
    
    /** BT Sending function **/
    if(g_byBTSendFlag){
        HC06.write(g_bySendPacket, BUFFER_SIZE);
        g_byBTSendFlag = 0;
    }
    
    
    
}


