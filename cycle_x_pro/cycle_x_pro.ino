//
// cycle-x-pro
//
// Embedded Software for Arduino Due
//
// Author 		Carlos Salamanca
// 				team14
//
// Date			9/22/16 9:40 AM
// Version		2.0.3
//
// Copyright	© Carlos Salamanca, 2016
// Licence		MIT
//
// See         README.md for references
//


#include <Arduino.h>

#include "globals.h"
#include "functions.h"
#include "testing.h"

/** Declare variables **/
struct timeStamp g_TimeStamp;
Floater32_t g_fMetric1;
Floater32_t g_fMetric2;
Floater32_t g_fMetric3;
Floater32_t g_fLatitude;
Floater32_t g_fLongitude;

uint8_t g_bySendPacket[BUFFER_SIZE];
uint8_t g_byStatus = 0x00;
int8_t g_byRecvPacket = -1;
uint16_t g_wOffsetTime = 0;
uint8_t g_byNextUpdate = 0;
uint8_t g_byMode = MODE_IDLE;

/** Setup Arduino objects **/

void setup() {
    HC06.begin(BAUD_RATE);
    
#ifdef TEST_CODE
    pinMode(PIN53, OUTPUT);
    pinMode(PIN51, OUTPUT);
    pinMode(PIN49, OUTPUT);
    pinMode(PIN47, INPUT);
#endif
    
}

/** Switches between modes of operation i.e the "OS" **/
void loop() {
    
    //listen for commands from App
    g_byRecvPacket = btListen();
    
    if (g_byRecvPacket > -1) {
        
        switch (g_byRecvPacket) {
                //Apply correct Mode of operation and System status
                
            case 0x45: //E - Enable Stream
            case 0x65:
                SET_STATUS(g_byStatus, RTS);
                SET_STATUS(g_byStatus, STREAM);
                g_wOffsetTime = millis();
                break;
            case 0x4B: //K - Send next sample
            case 0x6B:
                SET_STATUS(g_byStatus, RTS);
                break;
            case 0x51: //Q - End Session
            case 0x71:
                CLEAR_STATUS(g_byStatus, STREAM);
                CLEAR_STATUS(g_byStatus, RTS);
                CLEAR_STATUS(g_byStatus, NEW_SESSION);
                g_byMode = MODE_IDLE;
                break;
            case 0x52: //R - Reset ERPS
            case 0x72:
                g_byMode = MODE_IDLE;
                break;
            case 0x57: //W - New Solo Session
            case 0x77:
                g_byMode = MODE_SOLO;
                SET_STATUS(g_byStatus, NEW_SESSION);
            default:
                //do nothing
                __asm__("nop\n\t");
        }
    }
    //update data
    test_updateData();
    
    //Send message to App
    btSend();
    
}

/** Input and output functions **/
int8_t btListen() {
    if (HC06.available() > 0) return HC06.read();
    else return -1;
}

void btSend() {
    switch (g_byMode) {
            
            //This is a standby mode
        case MODE_IDLE:
            //TODO: Send battery percentage
            __asm__("nop\n\t");
            break;
            
            //Activated when ADS detects accident
        case MODE_ERPS:
            byteWrite(SEND_ERPS);
            break;
            
            //RTD Modes
        case MODE_SOLO:
        case MODE_RACE:
        case MODE_TRAINEE:
        case MODE_TRAINER:
            if (CHECK_STATUS(g_byStatus, STREAM)) {
                
                if (CHECK_STATUS(g_byStatus, RTS)) {
                    
                    if (CHECK_STATUS(g_byStatus, NEW_SESSION)) {
                        byteWrite(SEND_HEADER);
                        CLEAR_STATUS(g_byStatus, NEW_SESSION);
                    } else {
                        byteWrite(SEND_DATA);
                    }
                    
                    HC06.write(g_bySendPacket, BUFFER_SIZE);
                    CLEAR_STATUS(g_byStatus, RTS);
                }
            } else {
                CLEAR_STATUS(g_byStatus, RTS);
            }
            break;
        default:
            break;
    }
#ifdef TEST_CODE
    feedbackLED();
#endif
    
}

