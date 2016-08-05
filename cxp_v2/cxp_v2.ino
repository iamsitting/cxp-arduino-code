#include "globals.h"

struct timeStamp stamp;
Floater32_t g_fMetric1;
Floater32_t g_fMetric2;
Floater32_t g_fMetric3;

//x - stream, y  - new_session, z - rts
uint8_t sf = 0x00; //statusfield - 0x00000xyz

//locals
int8_t state = -1;
uint16_t offsetTime = 0;
uint8_t g_CsvHeadPacket[] = "time,met1,met2,met3";

void setup(){
	Serial.begin(BAUD_RATE);
	
}

void loop(){
	state = btListen();
	
	if(state > -1){
		
		switch(state){
			case 0x45:
			case 0x65:
				sf = 0x07; //0x111
				offsetTime = millis();
				break;
			case 0x4B:
			case 0x6B:
				sf |= 0x01; //rts = true
				break;
			case 0x51:
			case 0x71: //11111010
				sf &= 0xFA; //stream = false, rts = false
				break;
			default:
				//do nothing
				__asm__("nop\n\t");
		}
			
	}
	
	btSend();
	
}

int8_t btListen(){
	if(Serial.available() > 0) return Serial.read();
	else return -1;
}

void btSend(){
	if ((sf & 0x04) >> 2){ //if stream
		if (sf & 0x01){ //rts
			if ((sf & 0x02) >> 1){ //new_session
				byteWrite(SEND_HEADER);
				sf |= 0xFB; //new_session = false
			} else {
				getData();
				byteWrite(SEND_DATA);
			}
			sf |= 0xFE; //rts = false;
		}
	} else {
		sf |= 0xFE; //rts = false
	}
}

void getData(){
	g_fMetric1.bits32 = analogRead(POT) * 9.77517;
	g_fMetric2.bits32 = g_fMetric1.bits32 + 1;
	g_fMetric3.bits32 = g_fMetric2.bits32 + 1;
	getTime();
}

void getTime(){
	uint16_t elapsedMillis = millis() - offsetTime;
	uint16_t elapsedTime = (elapsedMillis)/1000;
	stamp.hour = numberOfHours(elapsedTime) & 0x00FF;
	stamp.minute = numberOfMinutes(elapsedTime) & 0x00FF;
	stamp.second.bits32 = numberOfSeconds(elapsedTime) + (float32_t)(elapsedMillis % 1000)/1000;
}

void byteWrite(uint8_t protocol){
	uint8_t packet[32];
	uint8_t i = 0;
	uint8_t length = 0;
	
	packet[0] = 0xA7;
	packet[1] = protocol;
	
	switch(protocol){
		case 1:
			packet[2] = stamp.hour;
			packet[3] = stamp.minute;
			packet[4] = stamp.second.by.te3;
			packet[5] = stamp.second.by.te2;
			packet[6] = stamp.second.by.te1;
			packet[7] = stamp.second.by.te0;
			
			packet[8] = g_fMetric1.by.te3;
			packet[9] = g_fMetric1.by.te2;
			packet[10] = g_fMetric1.by.te1;
			packet[11] = g_fMetric1.by.te0;
			
			packet[12] = g_fMetric2.by.te3;
			packet[13] = g_fMetric2.by.te2;
			packet[14] = g_fMetric2.by.te1;
			packet[15] = g_fMetric2.by.te0;
			
			packet[16] = g_fMetric3.by.te3;
			packet[17] = g_fMetric3.by.te2;
			packet[18] = g_fMetric3.by.te1;
			packet[19] = g_fMetric3.by.te0;
			
			packet[20] = 0x00; //terminator
			length = 21;
			break;
		case 2:
			length = sizeof(g_CsvHeadPacket) + 3;
			for(i=0; i<length-3; i++){
				packet[2+i] = g_CsvHeadPacket[i];
			}
			packet[length-1] = 0x00; //terminator
			break;
		default:
			break;
	}
	Serial.write(packet, length);
}

