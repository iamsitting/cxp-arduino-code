#include "globals.h"
#include "functions.h"

//We will write data processing functions here

void getData(){
	g_fMetric1.bits32 = (float) analogRead(POT) * 9.77517;
	g_fMetric2.bits32 = g_fMetric1.bits32 + 1;
	g_fMetric3.bits32 = g_fMetric2.bits32 + 1;
	getTime();
}

void getTime(){
	uint16_t elapsedMillis = millis() - g_wOffsetTime;
	uint16_t elapsedTime = (elapsedMillis)/1000;
	g_TimeStamp.hour = numberOfHours(elapsedTime) & 0x00FF;
	g_TimeStamp.minute = numberOfMinutes(elapsedTime) & 0x00FF;
	g_TimeStamp.second.bits32 = numberOfSeconds(elapsedTime) + (float32_t)(elapsedMillis % 1000)/1000;
}

void byteWrite(uint8_t protocol){
	uint8_t packet[BUFFER_SIZE];
	uint8_t i = 0;
	uint8_t c = 0;
	uint16_t checksum = 0;
	
	switch(protocol){
		case SEND_DATA:
			packet[0] = g_TimeStamp.hour;
			packet[1] = g_TimeStamp.minute;
			packet[2] = g_TimeStamp.second.by.te3;
			packet[3] = g_TimeStamp.second.by.te2;
			packet[4] = g_TimeStamp.second.by.te1;
			packet[5] = g_TimeStamp.second.by.te0;
			
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
		case SEND_HEADER:
			
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

			for(c = 0; c<19; c++){
				checksum += packet[c]; 
			}
			packet[19] = checksum & 0xFF;

			packet[20] = protocol;
			packet[21] = 0xA7;
				
			break;
		default:
			break;
	}

  for(i = 0; i < BUFFER_SIZE; i++){
    g_bySendPacket[i] = packet[i];
  }
}
