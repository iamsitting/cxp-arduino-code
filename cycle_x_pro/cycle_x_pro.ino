//
// cycle-x-pro
//
// Embedded Software for Arduino Due
//
// Author       Carlos Salamanca
//              team14
//
// Date         9/22/16 9:40 AM
// Version      4.0.1
//
// Copyright    © Carlos Salamanca, 2016
// Licence      MIT
//
// See         README.md for references
//


#include <Arduino.h>
#include <TinyGPS++.h>
#include <BMI160Gen.h>

#include "globals.h"
#include "functions.h"
#include "als.h"
#include "testing.h"
#include "trio.h"
#include "rtd.h"

/** Declare variables **/

//metrics
struct timeStamp g_TimeStamp;

//BT, Program
uint8_t g_bySendPacket[BUFFER_SIZE];
uint8_t g_byRecvPacket[BUFFER_SIZE];
uint8_t g_byBTSendFlag = 0;
uint8_t g_byBTRecvFlag = 0;
uint8_t g_byStatus = 0x00;
uint32_t g_wOffsetTime = 0;
uint8_t g_byNextUpdate = 0;
uint8_t g_byMode = MODE_IDLE;
uint32_t g_wIdleMillis = 0;
uint32_t g_wDataMillis = 0;
uint8_t g_byMisses = 0;

//ALS
volatile uint8_t g_byChangedToSimple = 0;
volatile uint32_t g_wLastDebounceTime = 0;
volatile uint8_t g_byFlashingPattern = 0;
uint32_t g_wPreviousMillis = 0;
uint32_t g_wPreviousMillis2 = 0;
uint8_t g_byALSPin1State = LOW;
uint8_t g_byALSPin2State = LOW;
uint8_t g_byALSPin3State = LOW;
uint8_t g_byUsoundLtPinState = LOW;
uint8_t g_byFlashingCount = 0;
uint8_t g_byBatteryLevel = 90;
uint8_t g_byThreat = 0;
uint8_t g_byBrakeCounter = 0;
uint8_t g_byBrakeLtPinState = 0;
uint8_t g_byRelayState = 0;
uint8_t g_byStateOfCharge = 0;

//TRIO
uint8_t g_byXbeeRecvPacket[XBEE_BUFFER_SIZE];
uint8_t g_byXbeeSendPacket[XBEE_BUFFER_SIZE];
uint8_t g_byXbeeSendFlag = 0;
uint8_t g_byXbeeRecvFlag = 0;
uint8_t g_byXbeeisConfig = 0;
uint8_t g_byXbeemisses = 0;
Floater32_t g_fOppSpeed;
Floater32_t g_fOppDistance;
Floater32_t g_fOppCalories;
Floater32_t g_fOppLongitude;
Floater32_t g_fOppLatitude;
uint8_t g_byOppERPS = 0;
uint8_t g_byUserName[NAME_SIZE];
uint8_t g_byOppUserName[NAME_SIZE];
uint8_t g_byMyTRIOid[ADDR_SIZE];
uint8_t g_byDestTRIOid[ADDR_SIZE];
uint8_t g_byTRIOisInit = 0;
uint8_t g_byTRIOisReady = 0;
uint8_t g_byWinning = 0;
uint8_t g_byReadyCount = 0;

//ADS & RTD
Floater32_t g_fSpeed;
Floater32_t g_fDistance;
Floater32_t g_fCalories;
Floater32_t g_fLatitude; 
Floater32_t g_fLongitude;
TinyGPSPlus gps;
BMI160GenClass Bmi;
uint32_t previousMillis_distance = 0;
uint32_t previousMillis_cal = 0;
uint32_t previousMillis_dre = 0;
uint32_t previousMillis_fad = 0;
uint16_t g_halfWeight = 0;
uint8_t g_byGPSMisses = 0;


/** Setup Arduino objects **/
//#define ALS_TEST
uint8_t ind = 0;
float32_t sine_test[32] = {512,611,707,796,873,937,984,1013,1023,1013,984,937,873,796,707,611,512,412,316,227,150,86,39,10,0,10,39,86,150,227,316,412};
uint8_t test = 0;

void setup() {
  uint8_t t = 0;
  /*
  g_byMyTRIOid[t++] = '4';
  g_byMyTRIOid[t++] = '0';
  g_byMyTRIOid[t++] = 'B';
  g_byMyTRIOid[t++] = 'D';
  
  g_byMyTRIOid[t++] = 'B';
  g_byMyTRIOid[t++] = '1';
  g_byMyTRIOid[t++] = 'E';
  g_byMyTRIOid[t++] = 'C';
  */
  g_byMyTRIOid[t++] = '4';
  g_byMyTRIOid[t++] = '0';
  g_byMyTRIOid[t++] = 'E';
  g_byMyTRIOid[t++] = '6';
  
  g_byMyTRIOid[t++] = '5';
  g_byMyTRIOid[t++] = 'A';
  g_byMyTRIOid[t++] = 'E';
  g_byMyTRIOid[t++] = '1';
  
    HC06.begin(BAUD_RATE);
    DEBUG.begin(9600);
#ifdef ENABLE_ALS    
    setupALS();
#endif
#ifdef ENABLE_RTD
    setupRTD();
#endif

#ifdef ENABLE_TRIO
    setupTrio();
#endif
  
}

/** Switches between modes of operation i.e the "OS" **/
void loop() {

#ifdef ENABLE_TRIO
    //0. configure Xbee
    if(CHECK_STATUS(g_byStatus, BTCON)){
      if(!g_byXbeeisConfig){
        DEBUG.println("Configuring...");
        XBeeConfigure();
        sendHandshake(); //This is what breaks the App's ProgressDialog
        g_byXbeeisConfig = 1;
      }
    }
#endif

#ifdef ENABLE_ALS    
    //0. Read GPS
    while(GP20U7.available() > 0){
      gps.encode(GP20U7.read());
    }
    //1. switch flashing pattern
    DEBUG.println("ALS Stuff");
#ifdef ENABLE_LEDS
    switchFlashingPattern();
    flashRearLEDS();
    //To be tested
    changeBrakeLight();
#endif
#endif
    
    DEBUG.println("BT Receive");
    //2. listen for commands from App
    BluetoothReceive();

    DEBUG.println("BT Parse");
    //3. Parse command/change mode M 
    if (g_byBTRecvFlag) {
        g_byBTRecvFlag = 0;
        BluetoothDeconstructMessage();
    }

    if(g_byMode == MODE_COACH || g_byMode == MODE_ATHLETE){
      DEBUG.print("    MODE   ");
      DEBUG.println(g_byMode);
      DEBUG.print("    Speed   ");
      DEBUG.println(g_fSpeed.bits32, DEC);
      DEBUG.print("    Status   ");
      DEBUG.println(g_byStatus, BIN);
      DEBUG.print("    ERPS!!   ");
      DEBUG.println(CHECK_STATUS(g_byStatus, ERPS));
      DEBUG.print("    Opp. Speed!!   ");
      DEBUG.println(g_fOppSpeed.bits32, DEC);
      DEBUG.print("    Seconds!!   ");
      DEBUG.println(g_TimeStamp.second.bits32, DEC);
      DEBUG.print("    Weight!!   ");
      DEBUG.println(g_halfWeight, DEC);
      DEBUG.print("    Name!!   ");
      DEBUG.write(g_byUserName, NAME_SIZE);
      DEBUG.print("    Dest!!   ");
      DEBUG.write(g_byDestTRIOid, ADDR_SIZE);
      DEBUG.write('\n');
    } else {
      DEBUG.print("    Seconds!!   ");
      DEBUG.println(g_TimeStamp.second.bits32, DEC);
    }

    //4. update data
    //updateData();
    if(g_byMode != MODE_COACH){
#ifndef TEST_CODE
    
      getUSThreat();
      updateData();
    
#else
    updateData2();
#endif
    }

#ifdef ENABLE_TRIO
    if(g_byMode == MODE_RACE || g_byMode == MODE_COACH || g_byMode == MODE_ATHLETE){ //add athlete and coach
      //5. receive trio
      XBeeReceive();
      
      //5. Listen for TRIO
      if(g_byXbeeRecvFlag){
        g_byXbeeRecvFlag = 0;
        XBeeDeconstructMessage();
      }
    }
      //6. Build & Send to Opponent
      XbeeSendMessage();
#endif
    //7. Send message to App
    BluetoothSend();

#ifdef ENABLE_RTD
    //8. Check ADS
    if(CHECK_STATUS(g_byStatus, POSS_ACC)){
      DEBUG.print("    CheckingFalseAlarm   ");
        checkFalseAlarm();
    }
#endif
    
}


