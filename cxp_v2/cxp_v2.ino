#include "globals.h"
#include "functions.h"
	/** Declare variables **/
struct timeStamp g_TimeStamp;
Floater32_t g_fMetric1;
Floater32_t g_fMetric2;
Floater32_t g_fMetric3;

uint8_t g_bySendPacket[BUFFER_SIZE];
uint8_t g_byStatus = 0x00;
int8_t g_byRecvPacket = -1;
uint16_t g_wOffsetTime = 0;

	/** Setup Arduino objects **/

void setup(){
	Serial.begin(BAUD_RATE);
}

	/** Switches between modes of operation i.e the "OS" **/
void loop(){
	g_byRecvPacket = btListen();
	
	if(g_byRecvPacket > -1){
		
		switch(g_byRecvPacket){
			case 0x45: //E
			case 0x65: //e
				g_byStatus |= 0x05; //0b101, stream = true, rts = true
				g_wOffsetTime = millis();
				break;
			case 0x4B: //K
			case 0x6B: //k
				g_byStatus |= 0x01; //rts = true
				break;
			case 0x51: //Q
			case 0x71: //q
				//xxxxxx000
				g_byStatus &= 0xF8; //stream = false, rts = false, new_session = false
				break;
			case 0x57: //W
			case 0x77: //w
				g_byStatus |= 0x02; //0bx1x, new_session = true
			default:
				//do nothing
				__asm__("nop\n\t");
		}
	}
	
	btSend();
	
}

	/** Input and output functions **/
int8_t btListen(){
	if(Serial.available() > 0) return Serial.read();
	else return -1;
}

void btSend(){
	if ((g_byStatus & 0x04) >> 2){ //if stream
		if (g_byStatus & 0x01){ //rts
			if ((g_byStatus & 0x02) >> 1){ //new_session
				byteWrite(SEND_HEADER);
				//0bxxxxxx0x
				g_byStatus &= 0xFD; //new_session = false
			} else {
				getData();
				byteWrite(SEND_DATA);
			}

			Serial.write(g_bySendPacket, BUFFER_SIZE);
			//0bxxxxxxx0
			g_byStatus &= 0xFE; //rts = false;
		}
	} else {
		g_byStatus &= 0xFE; //rts = false
	}
}

