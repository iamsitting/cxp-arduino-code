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
uint8_t headPacket[] = "time,met1,met2,met3";
uint8_t lenHeadPacket = sizeof(headPacket); //19

void setup(){
	Serial.begin(BAUD_RATE);
	
}

void loop(){
	state = btListen();
	
	if(state > -1){
		
		switch(state){
			case 0x45: //E
			case 0x65: //e
				sf |= 0x05; //0b101, stream = true, rts = true
				offsetTime = millis();
				break;
			case 0x4B: //K
			case 0x6B: //k
				sf |= 0x01; //rts = true
				break;
			case 0x51: //Q
			case 0x71: //q
        //11111000
				sf &= 0xF8; //stream = false, rts = false, new_session = false
				break;
      case 0x57: //W
      case 0x77: //w
        sf |= 0x03; //0b011, new_session = true
			default:
				//do nothing
				__asm__("nop\n\t");
		}
	  Serial.println(state);
    Serial.println(sf);
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
        //11111101
				sf &= 0xFD; //new_session = false
			} else {
				getData();
				byteWrite(SEND_DATA);
			}
     //11111110
			sf &= 0xFE; //rts = false;
		}
	} else {
		sf &= 0xFE; //rts = false
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
  uint8_t c = 0;
	uint8_t len = 0;
  uint16_t checksum = 0;
	
	switch(protocol){
		case 1:
			packet[0] = stamp.hour;
			packet[1] = stamp.minute;
			packet[2] = stamp.second.by.te3;
			packet[3] = stamp.second.by.te2;
			packet[4] = stamp.second.by.te1;
			packet[5] = stamp.second.by.te0;
			
			packet[6] = g_fMetric1.by.te3;
			packet[7] = g_fMetric1.by.te2;
			packet[8] = g_fMetric1.by.te1;
			packet[9] = g_fMetric1.by.te0;
			
			packet[10] = g_fMetric2.by.te3;
			packet[11] = g_fMetric2.by.te2;
			packet[12] = g_fMetric2.by.te1;
			packet[13] = g_fMetric2.by.te0;
		
			packet[14] = g_fMetric3.by.te3;
			packet[15] = g_fMetric3.by.te2;
			packet[16] = g_fMetric3.by.te1;
			packet[17] = g_fMetric3.by.te0;

      for(c = 0; c<18; c++){
        checksum += packet[c];
      }
      packet[18] = checksum & 0xFF;
      
      packet[19] = protocol;
      packet[20] = 0xA7;
      
			break;
		case 2:
			
      packet[0] = 0x74; //t
      packet[1] = 0x69; //i
      packet[2] = 0x6D; //m
      packet[3] = 0x65; //e
      packet[4] = 0x2C; //,
      
      packet[5] = 0x6D; //m
      packet[6] = 0x65; //e
      packet[7] = 0x74; //t
      packet[8] = 0x31; //1
      packet[9] = 0x2C; //,
      
      packet[10] = 0x6D; //m
      packet[11] = 0x65; //e
      packet[12] = 0x74; //t
      packet[13] = 0x32; //2
      packet[14] = 0x2C; //,
      packet[15] = 0x6D; //m
      packet[16] = 0x65; //e
      packet[17] = 0x74; //t
      packet[18] = 0x33; //3
      
      for(c = 0; c<18; c++){
        checksum += packet[c]; 
      }
      packet[19] = checksum & 0xFF;
      
      packet[20] = protocol;
      packet[21] = 0xA7;
			
			break;
		default:
			break;
	}
	Serial.write(packet, 32);
}

