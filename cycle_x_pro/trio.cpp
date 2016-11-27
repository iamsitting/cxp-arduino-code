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

uint8_t ATSLmisses = 0;
uint8_t ATHLmisses = 0;
/********************* Written by Fernando Romo *************************/
void XBeeBuildMessage(uint8_t protocol){
    uint8_t packet[XBEE_BUFFER_SIZE];
    uint8_t i = 0;
    uint8_t c = 0;
    uint16_t checksum = 0;
    
    switch (protocol) {
        case TRIO_INIT:
            packet[i++] = protocol;
            packet[i++] = CHECK_STATUS(g_byStatus, ERPS) >> ERPS;
            for(c = 0; c < NAME_SIZE; c++)
            {
              packet[i++] = g_byUserName[c];
            }
            
            break;
        case TRIO_RACE:
            packet[i++] = START_BYTE;
            packet[i++] = protocol;
            if(CHECK_STATUS(g_byStatus, ERPS)) {
              packet[i++] = TRIO_ERPS_1;
              packet[i++] = g_fLatitude.by.te0;
              packet[i++] = g_fLatitude.by.te1;
              packet[i++] = g_fLatitude.by.te2;
              packet[i++] = g_fLatitude.by.te3;
              packet[i++] = g_fLongitude.by.te0;
              packet[i++] = g_fLongitude.by.te1;
              packet[i++] = g_fLongitude.by.te2;
              packet[i++] = g_fLongitude.by.te3;
              //get time
            } else {
              packet[i++] = TRIO_ERPS_0;
              packet[i++] = g_fSpeed.by.te0;
              packet[i++] = g_fSpeed.by.te1;
              packet[i++] = g_fSpeed.by.te2;
              packet[i++] = g_fSpeed.by.te3;
              packet[i++] = g_fDistance.by.te0;
              packet[i++] = g_fDistance.by.te1;
              packet[i++] = g_fDistance.by.te2;
              packet[i++] = g_fDistance.by.te3;
              }
            
            break;
        case TRIO_ATHLETE:
            packet[i++] = START_BYTE;
            packet[i++] = protocol;
            if(CHECK_STATUS(g_byStatus, ERPS)) {
              packet[i++] = TRIO_ERPS_1;
              packet[i++] = g_fLatitude.by.te0;
              packet[i++] = g_fLatitude.by.te1;
              packet[i++] = g_fLatitude.by.te2;
              packet[i++] = g_fLatitude.by.te3;
              packet[i++] = g_fLongitude.by.te0;
              packet[i++] = g_fLongitude.by.te1;
              packet[i++] = g_fLongitude.by.te2;
              packet[i++] = g_fLongitude.by.te3;
              //get time
            } else {
              packet[i++] = TRIO_ERPS_0;
              packet[i++] = g_fSpeed.by.te0;
              packet[i++] = g_fSpeed.by.te1;
              packet[i++] = g_fSpeed.by.te2;
              packet[i++] = g_fSpeed.by.te3;
              packet[i++] = g_fDistance.by.te0;
              packet[i++] = g_fDistance.by.te1;
              packet[i++] = g_fDistance.by.te2;
              packet[i++] = g_fDistance.by.te3;
              packet[i++] = g_fCalories.by.te0;
              packet[i++] = g_fCalories.by.te1;
              packet[i++] = g_fCalories.by.te2;
              packet[i++] = g_fCalories.by.te3;
              
              packet[i++] = g_TimeStamp.hour;
              packet[i++] = g_TimeStamp.minute;
              packet[i++] = g_TimeStamp.second.by.te0;
              packet[i++] = g_TimeStamp.second.by.te1;
              packet[i++] = g_TimeStamp.second.by.te2;
              packet[i++] = g_TimeStamp.second.by.te3;
            }
            break;
        case TRIO_READY:
            packet[i++] = START_BYTE;
            packet[i++] = protocol;
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
    uint8_t i = 0;
    if(g_byXbeeRecvPacket[i++] == START_BYTE){
      DEBUG.println("DECONNING");
      g_byTRIOisReady = 1;
        switch (g_byXbeeRecvPacket[i++]) {
            case TRIO_INIT:
                DEBUG.println("INIT RECEIVED");
                break;
            case TRIO_READY:
                DEBUG.println("He's Ready");
                g_byTRIOisReady = 1;
                break;
            case TRIO_ATHLETE:
                if(g_byMode == MODE_COACH){
                  XBeeBuildMessage(TRIO_READY); //ACK
                }
                switch(g_byXbeeRecvPacket[i++]) {
                  case TRIO_ERPS_0:
                    g_fOppSpeed.by.te0 = g_byXbeeRecvPacket[i++];
                    g_fOppSpeed.by.te1 = g_byXbeeRecvPacket[i++];
                    g_fOppSpeed.by.te2 = g_byXbeeRecvPacket[i++];
                    g_fOppSpeed.by.te3 = g_byXbeeRecvPacket[i++];

                    g_fOppDistance.by.te0 = g_byXbeeRecvPacket[i++];
                    g_fOppDistance.by.te1 = g_byXbeeRecvPacket[i++];
                    g_fOppDistance.by.te2 = g_byXbeeRecvPacket[i++];
                    g_fOppDistance.by.te3 = g_byXbeeRecvPacket[i++];

                    g_fOppCalories.by.te0 = g_byXbeeRecvPacket[i++];
                    g_fOppCalories.by.te1 = g_byXbeeRecvPacket[i++];
                    g_fOppCalories.by.te2 = g_byXbeeRecvPacket[i++];
                    g_fOppCalories.by.te3 = g_byXbeeRecvPacket[i++];

                    g_TimeStamp.hour = g_byXbeeRecvPacket[i++];
                    g_TimeStamp.minute = g_byXbeeRecvPacket[i++];
                    g_TimeStamp.second.by.te0 = g_byXbeeRecvPacket[i++];
                    g_TimeStamp.second.by.te1 = g_byXbeeRecvPacket[i++];
                    g_TimeStamp.second.by.te2 = g_byXbeeRecvPacket[i++];
                    g_TimeStamp.second.by.te3 = g_byXbeeRecvPacket[i++];
                    break;
                  case TRIO_ERPS_1:
                    g_byOppERPS = TRIO_ERPS_1;
                    g_fOppLatitude.by.te0  = g_byXbeeRecvPacket[i++];
                    g_fOppLatitude.by.te1  = g_byXbeeRecvPacket[i++];
                    g_fOppLatitude.by.te2  = g_byXbeeRecvPacket[i++];
                    g_fOppLatitude.by.te3  = g_byXbeeRecvPacket[i++];

                    g_fOppLongitude.by.te0  = g_byXbeeRecvPacket[i++];
                    g_fOppLongitude.by.te1  = g_byXbeeRecvPacket[i++];
                    g_fOppLongitude.by.te2  = g_byXbeeRecvPacket[i++];
                    g_fOppLongitude.by.te3  = g_byXbeeRecvPacket[i++];

                    //getTime
                    break;
                  default:
                    break;
                }
                break;
            case TRIO_RACE:
                switch(g_byXbeeRecvPacket[i++]) {
                  case TRIO_ERPS_0:
                    g_fOppSpeed.by.te0 = g_byXbeeRecvPacket[i++];
                    g_fOppSpeed.by.te1 = g_byXbeeRecvPacket[i++];
                    g_fOppSpeed.by.te2 = g_byXbeeRecvPacket[i++];
                    g_fOppSpeed.by.te3 = g_byXbeeRecvPacket[i++];

                    g_fOppDistance.by.te0 = g_byXbeeRecvPacket[i++];
                    g_fOppDistance.by.te1 = g_byXbeeRecvPacket[i++];
                    g_fOppDistance.by.te2 = g_byXbeeRecvPacket[i++];
                    g_fOppDistance.by.te3 = g_byXbeeRecvPacket[i++];

                    if(g_fDistance.bits32 > g_fOppDistance.bits32){
                      g_byWinning = 1;
                    } else {
                      g_byWinning = 0;
                    }
                    break;
                  case TRIO_ERPS_1:
                    g_byOppERPS = TRIO_ERPS_1;
                    g_fOppLatitude.by.te0  = g_byXbeeRecvPacket[i++];
                    g_fOppLatitude.by.te1  = g_byXbeeRecvPacket[i++];
                    g_fOppLatitude.by.te2  = g_byXbeeRecvPacket[i++];
                    g_fOppLatitude.by.te3  = g_byXbeeRecvPacket[i++];

                    g_fOppLongitude.by.te0  = g_byXbeeRecvPacket[i++];
                    g_fOppLongitude.by.te1  = g_byXbeeRecvPacket[i++];
                    g_fOppLongitude.by.te2  = g_byXbeeRecvPacket[i++];
                    g_fOppLongitude.by.te3  = g_byXbeeRecvPacket[i++];
                    //getTime
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
        DEBUG.println("RECEIVE...");
        DEBUG.println(g_byXbeeRecvPacket[0], DEC);
        DEBUG.println(g_byXbeeRecvPacket[1], DEC);
        DEBUG.println(g_byXbeeRecvPacket[2], DEC);
        XBPRO.flush();
    }
}

void XbeeSendMessage(){
  DEBUG.println("SENDING....");
    switch(g_byMode) {
        case MODE_IDLE:
            if(g_byXbeeisConfig){
              if(!g_byTRIOisInit){
                DEBUG.println("Initialize");
                XBeeBuildMessage(TRIO_INIT);
                g_byTRIOisInit = 1;
              }
            }
            break;
        case MODE_ATHLETE:
            if(g_byTRIOisInit){
              if(g_byTRIOisReady){
                XBeeBuildMessage(TRIO_ATHLETE);
                g_byTRIOisReady = 0;
              } else {
                countMiss();
              }
            }
            break;
        case MODE_RACE:
            if(g_byTRIOisInit){
              if(g_byTRIOisReady){
                g_byTRIOisReady = 0;
                DEBUG.println("IN RACE MODE....");
                XBeeBuildMessage(TRIO_RACE);
              } else {
                countMiss();
                
                //XBeeBuildMessage(TRIO_READY);
              }
            }
            break;
        case MODE_COACH:
          if(g_byTRIOisInit && (g_byReadyCount > 5)){
            g_byReadyCount = 0;
            XBeeBuildMessage(TRIO_READY);
            } else {
              g_byReadyCount++;
            }
            break;
        default:
            __asm__("nop\n\t");
            break;
    }
    
    /** XBEE Sending function **/
    if(g_byXbeeSendFlag){
        XBPRO.write(g_byXbeeSendPacket, XBEE_BUFFER_SIZE);
        DEBUG.println("SENDING Packet:");
        DEBUG.println(g_byXbeeSendPacket[0], DEC);
        DEBUG.println(g_byXbeeSendPacket[1], DEC);
        DEBUG.println(g_byXbeeSendPacket[2], DEC);
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

uint8_t ATDL_RESET(){
  uint8_t packet[6];
  packet[0] = 'A';
  packet[1] = 'T';
  packet[2] = 'D';
  packet[3] = 'L';
  packet[4] = '0';
  packet[5] = 13;
  XBPRO.write(packet,6);
  delay(100);
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

uint8_t ATDL_READ(){
  uint8_t packet[5];
  uint8_t check[8];
  uint8_t i = 0;
  uint8_t j = 0;
  uint8_t success = 0;
  packet[i++] = 'A';
  packet[i++] = 'T';
  packet[i++] = 'D';
  packet[i++] = 'L';
  packet[i++] = 13;
  delay(200);
  XBPRO.readBytes(check, 8);
  DEBUG.println("ATDL_READ");
  DEBUG.write(check, 8);
  DEBUG.write('\n');
  for(j = 0; j < 8; j++){
    if(g_byDestTRIOid[j] != check[j]){
      return 0;
    }
  }
  return 1;
}

uint8_t ATDL(){
  uint8_t packet[12];
  uint8_t i = 0;
  uint8_t j = 0;
  packet[i++] = 'A';
  packet[i++] = 'T';
  packet[i++] = 'D';
  packet[i++] = 'L';

  DEBUG.println("ATDL");
  DEBUG.write(g_byDestTRIOid, ADDR_SIZE);
  DEBUG.write('\n');

  packet[i++] = g_byDestTRIOid[j++];
  packet[i++] = g_byDestTRIOid[j++];
  packet[i++] = g_byDestTRIOid[j++];
  packet[i++] = g_byDestTRIOid[j++];

  packet[i++] = g_byDestTRIOid[j++];
  packet[i++] = g_byDestTRIOid[j++];
  packet[i++] = g_byDestTRIOid[j++];
  packet[i++] = g_byDestTRIOid[j++];
  
  packet[12] = 13;
  XBPRO.write(packet, 12);
  delay(200);
  return ATcheckOK();
}

uint8_t ATMY(uint8_t addr){
  uint8_t packet[6];
  uint8_t i = 0;
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

uint8_t ATSL(){
  //TODO: Test this command
  uint8_t packet[5];
  packet[0] = 'A';
  packet[1] = 'T';
  packet[2] = 'S';
  packet[3] = 'L';
  packet[4] = 13;
  XBPRO.write(packet,5);
  delay(100);
  XBPRO.readBytes(g_byMyTRIOid, ADDR_SIZE);
  DEBUG.println("MY TRIO ID");
  DEBUG.write(g_byMyTRIOid, ADDR_SIZE);
  return 1;
}

uint8_t XBeeConfigure(){
  uint8_t successATSL = 0;
  uint8_t successATHL = 0;
  uint8_t done = 0;

  while(!done){
    if(!successATSL){
      //successATSL = XBeeATSL();
      if(XBeeATSL() || (ATSLmisses > 10)){
        successATSL = 1;
      }
      ATSLmisses++;
    }

    if(!successATHL){
      //successATHL = XBeeATHL();
      if(XBeeATHL() || (ATHLmisses > 5)){
        successATHL = 1;
      }
      ATHLmisses++;
    }

    if(successATSL && successATHL){
      ATSLmisses = 0;
      ATHLmisses = 0;
      done = 1;
      DEBUG.println("Configuration Complete!");
    }
  }
  
}

uint8_t XBeeATSL(){
  uint8_t success = 0;
   if(ATenterCommand()){
    if(ATSL()){
      DEBUG.println("Successfully Read Serial Low");
      success = 1;
    }
  }
  return success;
}

uint8_t XBeeATHL(){
  uint8_t success = 0;
  if(ATenterCommand()){
    if(ATDL_READ()){
        success = 1;
      } else {
        if(ATDL()){
          DEBUG.println("ATDL");
          if(ATWR()){
            DEBUG.println("ATWR");
            if(ATCN()){
              success = 1;
              Serial.println("Successfully Configured Destinantion Address");
            }
          }
        }
      }     
    }
  return success;
}

void countMiss(){
  g_byXbeemisses += 1;
  if(g_byXbeemisses  > 20){
    g_byXbeemisses  = 0;
    g_byTRIOisReady = 1;
    }
}
void setupTrio(){
    XBPRO.begin(XBEE_BAUD);
    delay(3000);
}
