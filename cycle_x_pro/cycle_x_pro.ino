//
// cycle-x-pro
//
// Embedded Software for Arduino Due
//
// Author       Carlos Salamanca
//              team14
//
// Date         9/22/16 9:40 AM
// Version      3.0.4
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

//TRIO
uint8_t g_byXbeeRecvPacket[XBEE_BUFFER_SIZE];
uint8_t g_byXbeeSendPacket[XBEE_BUFFER_SIZE];
uint8_t g_byXbeeSendFlag = 0;
uint8_t g_byXbeeRecvFlag = 0;
Floater32_t g_fOppSpeed;
Floater32_t g_fOppDistance;
Floater32_t g_fOppLongitude;
Floater32_t g_fOppLatitude;
uint8_t g_byUserName[NAME_SIZE];
uint8_t g_byOppUserName[NAME_SIZE];

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


void setup() {
    HC06.begin(BAUD_RATE);
    
    setupALS();
    setupRTD();
    
#ifdef TEST_CODE
    DEBUG.begin(9600);
    pinMode(PIN53, OUTPUT);
    pinMode(PIN51, OUTPUT);
    pinMode(PIN49, OUTPUT);
#endif

}

/** Switches between modes of operation i.e the "OS" **/
void loop() {

    //0. Read GPS
    while(GP20U7.available() > 0){
      gps.encode(GP20U7.read());
    }

    //1. switch flashing pattern
    DEBUG.println("ALS Stuff");
    //switchFlashingPattern();
    //flashRearLEDS();
    //To be tested
    //changeBrakeLight();
    
    DEBUG.println("BT Receive");
    //2. listen for commands from App
    BluetoothReceive();

    DEBUG.println("BT Parse");
    //3. Parse command/change mode M 
    if (g_byBTRecvFlag) {
        g_byBTRecvFlag = 0;
#ifdef TEST_CODE
        DEBUG.write(g_byRecvPacket[0]);
        DEBUG.write(g_byRecvPacket[1]);
        DEBUG.write('\n');
#endif
        BluetoothDeconstructMessage();
    }
    
//#ifdef TEST_CODE
    DEBUG.print("    MODE   ");
    DEBUG.println(g_byMode);
    DEBUG.print("    Speed   ");
    DEBUG.println(g_fSpeed.bits32, DEC);
    DEBUG.print("    Poss. Accident   ");
    DEBUG.println(CHECK_STATUS(g_byStatus, POSS_ACC));
    DEBUG.print("    ERPS!!   ");
    DEBUG.println(CHECK_STATUS(g_byStatus, ERPS));
    DEBUG.print("    Latitude!!   ");
    DEBUG.println(g_fLatitude.bits32);
    DEBUG.print("    Speed!!   ");
    DEBUG.println(g_fSpeed.bits32);
    
    //DEBUG.write('\n');
//#endif

    //4. update data
    //updateData();
    getUSThreat();
    updateData2();

    //5. receive trio
    //XBeeReceive();

    //5. Listen for TRIO
    if(g_byXbeeRecvFlag){
        g_byXbeeRecvFlag = 0;
        XBeeDeconstructMessage();
    }
    
    //6. Build & Send to Opponent
    //XbeeSendMessage();

    //7. Send message to App
    BluetoothSend();

    //8. Check ADS
    if(CHECK_STATUS(g_byStatus, POSS_ACC)){
      DEBUG.print("    CheckingFalseAlarm   ");
        checkFalseAlarm();
    }
    
}


