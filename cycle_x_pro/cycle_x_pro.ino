//
// cycle-x-pro
//
// Embedded Software for Arduino Due
//
// Author 		Carlos Salamanca
// 				team14
//
// Date			9/22/16 9:40 AM
// Version		2.0.11
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

/** Declare variables **/
struct timeStamp g_TimeStamp;
Floater32_t g_fMetric1;
Floater32_t g_fMetric2;
Floater32_t g_fMetric3;
Floater32_t g_fLatitude;
Floater32_t g_fLongitude;
uint8_t g_byBatteryLevel = 90;
uint8_t g_byThreat = 0;

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

uint8_t g_byChangedToSimple = 0;
uint32_t g_wLastDebounceTime = 0;
uint8_t g_byFlashingPattern = 0;
uint32_t g_wPreviousMillis = 0;
uint8_t g_byALSPin1State = LOW;
uint8_t g_byALSPin2State = LOW;
uint8_t g_byALSPin3State = LOW;
uint8_t g_byFlashingCount = 0;

/** Setup Arduino objects **/
//#define ALS_TEST

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
    
//#ifdef TEST_CODE
    DEBUG.print("    MODE   ");
    DEBUG.println(g_byMode);
    DEBUG.print("    STATUS   ");
    DEBUG.print(g_byStatus, BIN);
    //DEBUG.print("     pattern   ");
    //DEBUG.println(g_byFlashingPattern);
//#endif
    
    //update data
    test_updateData();
    
    //Send message to App
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
        case MODE_RACE:
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
                    CLEAR_STATUS(g_byStatus, RTS);
                }
                
            }
            
        }
            
            break;
        default:
            __asm__("nop\n\t");
            break;
    }
    
    if(g_byBTSendFlag){
        HC06.write(g_bySendPacket, BUFFER_SIZE);
        g_byBTSendFlag = 0;
    }
    
}



