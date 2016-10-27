//
//  trio.c
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 10/27/16.
//  Copyright © 2016 team14. All rights reserved.
//

#include <Arduino.h>
#include "types.h"
#include "globals.h"
#include "trio.h"

/********************* Written by Fernando Romo *************************/
void XBeeBuildMessage(){
    //TODO: write this function
}

void XBeeDeconstructMessage(){
    if(g_byXbeeRecvPacket[0] == START_BYTE){
        switch (g_byXbeeRecvPacket[1]) {
            case TRIO_INIT:
                //do
                break;
            case TRIO_TRAIN:
                if(g_byXbeeRecvPacket[2] == TRIO_ERPS_0){
                    
                    if(g_byXbeeRecvPacket[3] == TRIO_ATHLETE){
                        //Do not recv
                    } else if(g_byXbeeRecvPacket[3] == TRIO_COACH){
                        g_fOppSpeed.by.te0 = g_byXbeeRecvPacket[4];
                        g_fOppSpeed.by.te1 = g_byXbeeRecvPacket[5];
                        g_fOppSpeed.by.te2 = g_byXbeeRecvPacket[6];
                        g_fOppSpeed.by.te3 = g_byXbeeRecvPacket[7];
                        
                        g_fOppDistance.by.te0 = g_byXbeeRecvPacket[8];
                        g_fOppDistance.by.te1 = g_byXbeeRecvPacket[9];
                        g_fOppDistance.by.te2 = g_byXbeeRecvPacket[10];
                        g_fOppDistance.by.te3 = g_byXbeeRecvPacket[11];
                    }
                } else if(g_byXbeeRecvPacket[2] == TRIO_ERPS_1){
                    g_fOppLatitude.by.te0 = g_byXbeeRecvPacket[3];
                    g_fOppLatitude.by.te1 = g_byXbeeRecvPacket[4];
                    g_fOppLatitude.by.te2 = g_byXbeeRecvPacket[5];
                    g_fOppLatitude.by.te3 = g_byXbeeRecvPacket[6];
                    
                    g_fOppLongitude.by.te0 = g_byXbeeRecvPacket[7];
                    g_fOppLongitude.by.te1 = g_byXbeeRecvPacket[8];
                    g_fOppLongitude.by.te2 = g_byXbeeRecvPacket[9];
                    g_fOppLongitude.by.te3 = g_byXbeeRecvPacket[10];
                    
                    //erps time
                }
                break;
            case TRIO_RACE:
                if(g_byXbeeRecvPacket[2] == TRIO_ERPS_0){
                    g_fOppSpeed.by.te0 = g_byXbeeRecvPacket[3];
                    g_fOppSpeed.by.te1 = g_byXbeeRecvPacket[4];
                    g_fOppSpeed.by.te2 = g_byXbeeRecvPacket[5];
                    g_fOppSpeed.by.te3 = g_byXbeeRecvPacket[6];
                    
                    g_fOppDistance.by.te0 = g_byXbeeRecvPacket[7];
                    g_fOppDistance.by.te1 = g_byXbeeRecvPacket[8];
                    g_fOppDistance.by.te2 = g_byXbeeRecvPacket[9];
                    g_fOppDistance.by.te3 = g_byXbeeRecvPacket[10];
                } else if(g_byXbeeRecvPacket[2] == TRIO_ERPS_1){
                    g_fOppLatitude.by.te0 = g_byXbeeRecvPacket[3];
                    g_fOppLatitude.by.te1 = g_byXbeeRecvPacket[4];
                    g_fOppLatitude.by.te2 = g_byXbeeRecvPacket[5];
                    g_fOppLatitude.by.te3 = g_byXbeeRecvPacket[6];
                    
                    g_fOppLongitude.by.te0 = g_byXbeeRecvPacket[7];
                    g_fOppLongitude.by.te1 = g_byXbeeRecvPacket[8];
                    g_fOppLongitude.by.te2 = g_byXbeeRecvPacket[9];
                    g_fOppLongitude.by.te3 = g_byXbeeRecvPacket[10];
                    
                    //erps time
                }
                break;
            default:
                break;
        }
        
    }
}