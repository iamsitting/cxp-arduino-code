#include "globals.h"
#include "functions.h"
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

void setup(){
	Serial.begin(BAUD_RATE);
}

	/** Switches between modes of operation i.e the "OS" **/
void loop(){
	
	//listen for commands from App
	g_byRecvPacket = btListen();
	
	if(g_byRecvPacket > -1){
		
		switch(g_byRecvPacket){
			//Apply correct Mode of operation and System status
			
			case 0x45: //E - Enable Stream
				SET_STATUS(g_byStatus, RTS);
				SET_STATUS(g_byStatus, STREAM);
				g_wOffsetTime = millis();
				break;
			case 0x4B: //K - Send next sample
				SET_STATUS(g_byStatus, RTS);
				break;
			case 0x51: //Q - End Session
				CLEAR_STATUS(g_byStatus, STREAM);
				CLEAR_STATUS(g_byStatus, RTS);
				CLEAR_STATUS(g_byStatus, NEW_SESSION);
				g_byMode = MODE_IDLE;
				break;
			case 0x52: //R - Reset ERPS
				g_byMode = MODE_IDLE;
				break;
			case 0x57: //W - New Solo Session
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
int8_t btListen(){
	if(Serial.available() > 0) return Serial.read();
	else return -1;
}

void btSend(){
	switch(g_byMODE){
		
		//This is a standby mode
		case MODE_IDLE:
			//TODO: Send battery percentage
			_asm_("nop\n\t");
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
			if (CHECK_STATUS(g_byStatus, STREAM)){
		
				if (CHECK_STATUS(g_byStatus, RTS)){
			
					if (CHECK_STATUS(g_byStatus, NEW_SESSION)){
						byteWrite(SEND_HEADER);
						CLEAR_STATUS(g_byStatus, NEW_SESSION);
					} else {
						byteWrite(SEND_DATA);
					}

					Serial.write(g_bySendPacket, BUFFER_SIZE);
					CLEAR_STATUS(g_byStatus, RTS);
				}
			} else {
				CLEAR_STATUS(g_byStatus, RTS);
			}
			break;
		default:
			break;
}

