#include <Wire.h>

#define ADDRESS 0x08
boolean on = true;
int softOff = 0;
long sleepTime = 0;//?

boolean nextmy1 = false;
boolean nextmy2 = false;
boolean sleepConfirm = false;
byte resp = 0;
byte sleep1 = 0;
byte sleep2 = 0;

void setup(){
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  Wire.begin(ADDRESS);
  Wire.onReceive(rec);
  Wire.onRequest(req);
  Serial.begin(9600);
}

void loop(){
  delay(1000);
  if(softOff>0){
    softOff--;
  } else {
    if(sleepTime>0){
      digitalWrite(13, false);
      digitalWrite(12, false);
      sleepTime--;
    }else{
      digitalWrite(13, true);
      digitalWrite(12, true);
    }
  }
}

void rec(int bc){
  
  byte t;
  while(Wire.available()){
    t = Wire.read();
  }
  
  if(nextmy1){sleep1 = t;nextmy1 = false;}else{
    if(nextmy2){
      sleep2 = t;
      nextmy2 = false;
      //if(((sleep1 * 250 + sleep2) > 0)&&(sleepTime == 0)&&(softOff == 0)) {sleepTime = sleep1 * 250 + sleep2;softOff=20;}
    }else{
      switch(t){
        case 251: resp = analogRead(0) / 4;break;
        case 252: resp = analogRead(1) - 499;break;
        case 253: nextmy1 = true;break;
        case 254: nextmy2 = true;break;
        case 255: sleepConfirm = true;break;
      }
    }
  }
  if(sleepConfirm){
    if(((sleep1 * 250 + sleep2) > 0)&&(sleepTime == 0)&&(softOff == 0)) {sleepTime = sleep1 * 250 + sleep2;softOff=20;};
    sleepConfirm = false;
  }
  
}

void req(){
  Wire.write(resp);
  Serial.println(resp);
  resp = 0;
}
