#include <Wire.h>

#define ADDRESS 0x08
boolean on = true;
int softOff = 0;
long sleepTime = 0;//?

boolean nextmy1 = false;
boolean nextmy2 = false;
boolean sleepConfirm = false;
byte resp = 0;
int tamp[25];
int ci = 0;
int amp = 0;
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
  delay(40);
  tamp[ci] = analogRead(1);
  if(ci >= 24) {
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
    amp = 0;
    for (int i = 0; i <= 24; i++) {
       amp += tamp[i];
    }
    amp = amp / 25;
    ci = -1;
    //Serial.println(amp);
  }
  ci++;
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
    }else{
      switch(t){
        case 251: resp = analogRead(0) / 4;break;
        case 252: resp = amp - 508;break;
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
  Serial.println(resp);//required, it's a current magic
  resp = 0;
}
