#include <SoftwareSerial.h>

SoftwareSerial softSerial(5, 6);
const int trigger = 3;
const int echo = 4;
const int moto1Enable = 9;
const int moto1in1 = 19;
const int moto1in2 = 17;
const int moto2Enable = 10;
const int moto2in1 = 16;
const int moto2in2 = 18;

float sDist = 0;
long sTime = 0;
float sMax = 0;
float sMin = 0;

String readString;

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  int incomingBytes = softSerial.available();

  if (incomingBytes >= 6) {
    handleIncomingMsg(incomingBytes);
  }
}

void handleIncomingMsg(int incomingBytes) {
  char cmd[6];
  
  for (int i = 0; i < incomingBytes; i++) {
    cmd[i] = softSerial.read(); 
  }

  // 2
  if (strcmp(cmd, "CMD020") == 0) {
    setMotor(0, false, 2);
  }
  else if (strcmp(cmd, "CMD022") == 0) {
    setMotor(255, false, 2);
  }    
  else if (strcmp(cmd, "CMD122") == 0) {
    setMotor(255, true, 2);
  }    
  // 1
  else if (strcmp(cmd, "CMD010") == 0) {
    setMotor(0, false, 1);
  }  
  else if (strcmp(cmd, "CMD012") == 0) {
    setMotor(255, false, 1);
  }
  else if (strcmp(cmd, "CMD112") == 0) {
    setMotor(255, true, 1);
  }
  // others
  else if (strcmp(cmd, "CMDLOK") == 0) {
      digitalWrite(trigger, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigger, LOW);
      sTime = pulseIn(echo, HIGH);
      sDist = (sTime / 58.0);
      sMax = sDist + (0.3 + 0.01 * sDist);
      sMin = sDist - (0.3 + 0.01 * sDist);
      softSerial.print("[1:<" + String(sMin) + "|" + String(sMax) + ">]");
  }  
  else if (strcmp(cmd, "CMDWHO") == 0) {
    softSerial.print("[1:jeep]");
  }
}

void stopMotors() {
  setMotor(0, false, 1);
  setMotor(0, false, 2);
}

void setMotor(int speed, boolean reverse, int moto)
{
  if (moto == 1) {
    analogWrite(moto1Enable, speed);
    digitalWrite(moto1in1, !reverse);
    digitalWrite(moto1in2, reverse);    
  } else if (moto == 2) {
    analogWrite(moto2Enable, speed);
    digitalWrite(moto2in1, !reverse);
    digitalWrite(moto2in2, reverse); 
  }
}

