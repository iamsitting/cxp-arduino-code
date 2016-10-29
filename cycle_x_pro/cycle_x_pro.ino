//
// cycle-x-pro
//
// Embedded Software for Arduino Due
//
// Author 		Carlos Salamanca
// 				team14
//
// Date			9/22/16 9:40 AM
// Version		2.2.1
//
// Copyright	© Carlos Salamanca, 2016
// Licence		MIT
//
// See         README.md for references
//


#include <Arduino.h>

#include "globals.h"
#include "functions.h"
#include "als.h"
#include "testing.h"
#include "trio.h"

/** Declare variables **/

//metrics
struct timeStamp g_TimeStamp;
Floater32_t g_fSpeed;
Floater32_t g_fDistance;
Floater32_t g_fCalories;
Floater32_t g_fLatitude;
Floater32_t g_fLongitude;
uint8_t g_byBatteryLevel = 90;
uint8_t g_byThreat = 0;

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
uint8_t g_byChangedToSimple = 0;
uint32_t g_wLastDebounceTime = 0;
uint8_t g_byFlashingPattern = 0;
uint32_t g_wPreviousMillis = 0;
uint8_t g_byALSPin1State = LOW;
uint8_t g_byALSPin2State = LOW;
uint8_t g_byALSPin3State = LOW;
uint8_t g_byFlashingCount = 0;

//TRIO
uint8_t g_byXbeeRecvPacket[XBEE_BUFFER];
uint8_t g_byXbeeSendFlag = 0;
uint8_t g_byXbeeRecvFlag = 0;
Floater32_t g_fOppSpeed;
Floater32_t g_fOppDistance;
Floater32_t g_fOppLongitude;
Floater32_t g_fOppLatitude;


/** Setup Arduino objects **/
//#define ALS_TEST
uint8_t ind = 0;
float32_t sine_test[32] = {512,611,707,796,873,937,984,1013,1023,1013,984,937,873,796,707,611,512,412,316,227,150,86,39,10,0,10,39,86,150,227,316,412};


void setup() {
    HC06.begin(BAUD_RATE);
    
#ifdef ALS_TEST
    setupALS();
#endif
    
#ifdef TEST_CODE
    DEBUG.begin(9600);
    pinMode(PIN53, OUTPUT);
    pinMode(PIN51, OUTPUT);
    pinMode(PIN49, OUTPUT);
#endif

}

/** Switches between modes of operation i.e the "OS" **/
void loop() {

    //1. switch flashing pattern
    switchFlashingPattern();

    //2. listen for commands from App
    btListen();
    
    //3. Parse command/change mode
    if (g_byBTRecvFlag) {
        g_byBTRecvFlag = 0;
#ifdef TEST_CODE
        DEBUG.write(g_byRecvPacket[0]);
        DEBUG.write(g_byRecvPacket[1]);
        DEBUG.write('\n');
#endif
        btParse();
    }
    
//#ifdef TEST_CODE
    DEBUG.print("    MODE   ");
    DEBUG.println(g_byMode);
    DEBUG.print("    STATUS   ");
    DEBUG.print(g_byStatus, BIN);
    //DEBUG.print("     pattern   ");
    //DEBUG.println(g_byFlashingPattern);
//#endif
    
    //4. update data
    updateData();
    
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
    btSend();
    
}

void setupALS(){
    pinMode(ALSPIN1, OUTPUT);
    pinMode(ALSPIN2, OUTPUT);
    pinMode(ALSPIN3, OUTPUT);
    
    pinMode(ALS_BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(ALS_BUTTON_PIN), ALSButton_isr, RISING);
}

/** Input and output functions **/
void btListen() {
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

void btSend() {
    switch (g_byMode) {
            
            //This is a standby mode
        case MODE_IDLE:
        {
            if(CHECK_STATUS(g_byStatus, BTCON)){
                uint16_t currentMillis = millis();
                if(currentMillis - g_wIdleMillis > THREE_SECONDS) {
                    g_wIdleMillis = currentMillis;
                    byteWrite(SEND_IDLE);
                }
            }
        }
            
            
            //__asm__("nop\n\t");
            break;
            
            //Activated when ADS detects accident
        case MODE_ERPS:
        {
            if(!CHECK_STATUS(g_byStatus, ERPS)){
                byteWrite(SEND_ERPS);

            } else{
                uint16_t currentMillis = millis();
                if(currentMillis - g_wIdleMillis > TEN_SECONDS) {
                    g_wIdleMillis = currentMillis;
                    byteWrite(SEND_IDLE);

                }
            }
        }
            
            
            break;
            
            //RTD Modes
        case MODE_SOLO:
        case MODE_TRAINEE:
        case MODE_TRAINER:
        {
            uint16_t currentMillis = millis();
            if(currentMillis - g_wDataMillis > 100) {
                g_wDataMillis = currentMillis;
                if (CHECK_STATUS(g_byStatus, RTS)) {
                    
                    if (CHECK_STATUS(g_byStatus, NEW_SESSION)) {
                        g_wOffsetTime = millis();
                        byteWrite(SEND_HEADER);
                        CLEAR_STATUS(g_byStatus, NEW_SESSION);
                    } else {
                        byteWrite(SEND_DATA);
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
                        byteWrite(SEND_HEADER);
                        CLEAR_STATUS(g_byStatus, NEW_SESSION);
                    } else {
                        byteWrite(SEND_RACE);
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
    
    /** BT Sending function **/
    if(g_byBTSendFlag){
        HC06.write(g_bySendPacket, BUFFER_SIZE);
        g_byBTSendFlag = 0;
    }
    
    
    
}

void XBeeReceive(){
    if(XBPRO.available() > 0){
        XBPRO.readBytes(g_byXbeeRecvPacket, XBEE_BUFFER);
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
        XBPRO.write(g_byXbeeSendPacket, XBEE_BUFFER);
        g_byXbeeSendFlag = 0;
    }
}


