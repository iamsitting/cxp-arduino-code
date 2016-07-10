#define POT 7
#define BUFF_SIZE 32

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)  

bool stream = false;
bool rts = false; //ready to send
bool new_session = false; //newsession
int state = -1;
long offsetTime = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  state = btListen();

  if ( state > -1) { //buffer not empty

    switch (state) {
      case 0x45: //E in ASCII
      case 0x65: //e
        stream = true;
        rts = true;
        new_session = true;
        offsetTime = millis();
        break;
      case 0x4b: //K
      case 0x6b: //k
        rts = true;
        break;
      case 0x51: //Q
      case 0x71: //q
        stream = false;
        rts = false;
        break;
      default:
        //do nothing
        __asm__("nop\n\t");
    }
  }

  btSend();
}

float getResistance() {
  //10K Pot
  float retVal = analogRead(POT) * 9.77517; //0-10000;
  return retVal;
}

int btListen() {
  if (Serial.available() > 0) {
    return Serial.read();
  }
  return -1;
}

void btSend() {

  String valStr = "";

  if (stream) {
    if (rts) {

      if (new_session) {
        valStr = getCSVHeader();
        new_session = false;
      } else {
        valStr = getData();
      }

      byteWrite(valStr, valStr.length()+1);
      //Serial.println(valStr);
      rts = false;
    }
  } else {
    if (rts) rts = false;
  }
}

String getCSVHeader() {
  return "speed,time,met1,met2";
}

String getData() {
  float val = getResistance();
  String retStr = String(val++, 1) + ","; //speed
  retStr += String(getTime()) + ","; //time
  retStr += String(val++, 1) + ","; //met1
  retStr += String(val, 1); //met2
  return retStr;
}

String getTime(){
  String retStr = "";
  long elapsedMillis = millis() - offsetTime; 
  long elapsedTime = (elapsedMillis)/1000;
  int hours = numberOfHours(elapsedTime);
  int minutes = numberOfMinutes(elapsedTime);
  float seconds = numberOfSeconds(elapsedTime) + (float)(elapsedMillis % 1000)/1000;

  retStr = String(hours)+":"+String(minutes)+":"+String(seconds,2);
  return retStr;
  
}
void byteWrite(String str, int len) {
  byte buff[len];
  str.getBytes(buff, len);
  //for(int i=0; i < len; i++){
  //Serial.print(buff[i]);
  //}
  Serial.write(buff, len);
}

