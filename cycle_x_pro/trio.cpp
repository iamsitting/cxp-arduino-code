//
//  trio.cpp
//  cycle-x-pro
//
//  Created by Carlos Salamanca on 10/27/16.
//  Copyright © 2016 team14. All rights reserved.
//

#include <Arduino.h>
#include "types.h"
#include "globals.h"
#include "functions.h"
#include "trio.h"
#include "rtd.h"

/********************* Written by Fernando Romo *************************/
void XBeeBuildMessage(uint8_t protocol){
    uint8_t packet[XBEE_BUFFER_SIZE];
    uint8_t i = 0;
    uint8_t c = 0;
    uint16_t checksum = 0;
    
    switch (protocol) {
        case TRIO_INIT:
            packet[i++] = TRIO_INIT;
            packet[i++] = CHECK_STATUS(g_byStatus, ERPS);
            packet[i++] = 'C';
            packet[i++] = 'A';
            packet[i++] = 'R';
            packet[i++] = 'L';
            packet[i++] = 'O';
            packet[i++] = 'S';
            packet[i++] = '7';
            packet[i++] = '8';
            break;
        case TRIO_TRAIN:
            if(g_byMode == MODE_ATHLETE){
                //if not ERPS
                if(!CHECK_STATUS(g_byStatus, ERPS)){
                    packet[i++] = START_BYTE;
                    packet[i++] = TRIO_ERPS_0;
                    packet[i++] = g_byMode; //TODO: verify
                    packet[i++] = g_fSpeed.by.te0;
                    packet[i++] = g_fSpeed.by.te1;
                    packet[i++] = g_fSpeed.by.te2;
                    packet[i++] = g_fSpeed.by.te3;
                    packet[i++] = g_fDistance.by.te0;
                    packet[i++] = g_fDistance.by.te1;
                    packet[i++] = g_fDistance.by.te2;
                    packet[i++] = g_fDistance.by.te3;
                } else {
                    //if ERPS
                    packet[i++] = START_BYTE;
                    packet[i++] = g_byMode; //TODO: verify
                    packet[i++] = TRIO_ERPS_1; //my_erps
                    packet[i++] = g_fLatitude.by.te0;
                    packet[i++] = g_fLatitude.by.te1;
                    packet[i++] = g_fLatitude.by.te2;
                    packet[i++] = g_fLatitude.by.te3;
                    packet[i++] = g_fLongitude.by.te0;
                    packet[i++] = g_fLongitude.by.te1;
                    packet[i++] = g_fLongitude.by.te2;
                    packet[i++] = g_fLongitude.by.te3;
                    //payload[11] = my_ERPS_time;
                }
            }
            break;
        case TRIO_RACE:
            if(!CHECK_STATUS(g_byStatus, ERPS)){
                packet[i++] = START_BYTE;
                packet[i++] = TRIO_RACE;
                packet[i++] = TRIO_ERPS_0;
                packet[i++] = g_fSpeed.by.te0;
                packet[i++] = g_fSpeed.by.te1;
                packet[i++] = g_fSpeed.by.te2;
                packet[i++] = g_fSpeed.by.te3;
                packet[i++] = g_fDistance.by.te0;
                packet[i++] = g_fDistance.by.te1;
                packet[i++] = g_fDistance.by.te2;
                packet[i++] = g_fDistance.by.te3;
            } else {
                packet[i++] = START_BYTE;
                packet[i++] = g_byMode; //TODO: verify
                packet[i++] = TRIO_ERPS_1; //my_erps
                packet[i++] = g_fLatitude.by.te0;
                packet[i++] = g_fLatitude.by.te1;
                packet[i++] = g_fLatitude.by.te2;
                packet[i++] = g_fLatitude.by.te3;
                packet[i++] = g_fLongitude.by.te0;
                packet[i++] = g_fLongitude.by.te1;
                packet[i++] = g_fLongitude.by.te2;
                packet[i++] = g_fLongitude.by.te3;
                //payload[11] = my_ERPS_time;
            }
            break;
        default:
            break;
    }
    for(i = 0; i < XBEE_BUFFER_SIZE; i++){
        g_byXbeeSendPacket[i] = packet[i];
    }
    
    g_byXbeeSendFlag = 1;
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

void XBeeReceive(){
    if(XBPRO.available() > 0){
        XBPRO.readBytes(g_byXbeeRecvPacket, XBEE_BUFFER_SIZE);
        g_byXbeeRecvFlag = 1;
    }
}

void XbeeSendMessage(){
    //TODO: add cases
    switch(g_byMode) {
        case MODE_IDLE:
            __asm__("nop\n\t");
            break;
        default:
            __asm__("nop\n\t");
            break;
    }
    
    /** XBEE Sending function **/
    if(g_byXbeeSendFlag){
        XBPRO.write(g_byXbeeSendPacket, XBEE_BUFFER_SIZE);
        g_byXbeeSendFlag = 0;
    }
}

uint8_t ATcheckOK(){
  uint8_t packet[3];
  uint8_t success = 0;
  XBPRO.readBytes(packet, 3);
  if(packet[0] == 'O')
    if(packet[1] == 'K')
      success = 1;
  return success;
}

uint8_t ATCN(){
  uint8_t packet[5];
  packet[0] = 'A';
  packet[1] = 'T';
  packet[2] = 'C';
  packet[3] = 'N';
  packet[4] = 13; //<ENTER>
  XBPRO.write(packet, 5);
  delay(100);
  return ATcheckOK();
}

uint8_t ATenterCommand(){
  uint8_t packet[3];
  packet[0] = '+';
  packet[1] = '+';
  packet[2] = '+';
  delay(GUARD_TIME);
  XBPRO.write(packet, 3);
  delay(GUARD_TIME);
  return ATcheckOK();
}

uint8_t ATDH(uint8_t addr){
  uint8_t packet[6];
  packet[0] = 'A';
  packet[1] = 'T';
  packet[2] = 'D';
  packet[3] = 'H';
  packet[4] = addr;
  packet[5] = 13;
  XBPRO.write(packet, 6);
  delay(100);
  return ATcheckOK();
}

uint8_t ATDL(uint8_t addr){
  uint8_t packet[6];
  packet[0] = 'A';
  packet[1] = 'T';
  packet[2] = 'D';
  packet[3] = 'L';
  packet[4] = addr;
  packet[5] = 13;
  XBPRO.write(packet, 6);
  delay(100);
  return ATcheckOK();
}

uint8_t ATMY(uint8_t addr){
  uint8_t packet[6];
  packet[0] = 'A';
  packet[1] = 'T';
  packet[2] = 'M';
  packet[3] = 'Y';
  packet[4] = addr;
  packet[5] = 13;
  XBPRO.write(packet, 6);
  delay(100);
  return ATcheckOK();
}

uint8_t ATWR(){
  uint8_t packet[5];
  packet[0] = 'A';
  packet[1] = 'T';
  packet[2] = 'W';
  packet[3] = 'R';
  packet[4] = 13;
  XBPRO.write(packet,5);
  delay(100);
  return ATcheckOK();
}

uint8_t XBeeConfigure(){
  uint8_t success = 0;
  if(ATenterCommand())
    if(ATDH(0))
      if(ATDL(g_byDestTRIOid))
        if(ATMY(g_byMyTRIOid))
          if(ATWR())
            if(ATCN())
              success = 1;
  return success;
}
void setupTrio(){
    XBPRO.begin(XBEE_BAUD);
}
