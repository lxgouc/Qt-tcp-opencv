#include <PID_v1.h>

const int EA=9;
const int EB=10;
const int pinLa=7;
const int pinLb=8;
const int pinRa=12;
const int pinRb=13;
const int trigpin=4;
const int echopin=5;

double SetpointL=20, SetpointR=20;
double Kp=0.6, Ki=5, Kd=0;
double outputL, outputR;
double inputL, inputR, speedvalueL, speedvalueR;
bool DirectionL, DirectionR;
bool DirectionlastL, DirectionlastR;
PID pidL(&inputL,&outputL,&SetpointL,Kp,Ki,Kd,DIRECT);
PID pidR(&inputR,&outputR,&SetpointR,Kp,Ki,Kd,DIRECT);

void setup() {
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  
  initleftwheel();
  initrightwheel();
  
  attachInterrupt(0,wheelspeedL,CHANGE);
  attachInterrupt(1,wheelspeedR,CHANGE);

  Serial.begin(19200);
}

void loop() {
  char key;
  if(Serial.available()>0)
  {   
      key=char(Serial.read());
      Serial.println(key);
  }
  else 
    return;

  switch(key)
  {
    case 'L':
      left();
      break;
    case 'R':
      right();
      break;
    case 'F':
      forbac();
      break;
    case 'B':
      back();
      break;
    case 'S':
      stopfb();
      break;
    default:
      break; 
  }

  inputL=abs(speedvalueL);
  inputR=abs(speedvalueR);

  if(pidL.Compute())
    speedvalueL=0;
  if(pidR.Compute())
    speedvalueR=0;

}

void initleftwheel()
{
  pinMode(EA,OUTPUT);
  pinMode(pinLa,OUTPUT);
  pinMode(pinLb,OUTPUT);
  pidL.SetMode(AUTOMATIC);
  pidL.SetSampleTime(40);
}

void initrightwheel()
{
  pinMode(EB,OUTPUT);
  pinMode(pinRa,OUTPUT);
  pinMode(pinRb,OUTPUT);
  pidR.SetMode(AUTOMATIC);
  pidR.SetSampleTime(40);
}

void wheelspeedL()
{
  if(DirectionL==DirectionlastL)
  {
    if(DirectionL)
      speedvalueL++;
    else
      speedvalueL--;
  }
  else
  {
    speedvalueL=0;
    if(DirectionL)
      speedvalueL++;
    else
      speedvalueL--;
  }
  DirectionlastL=DirectionL;
}

void wheelspeedR()
{
   if(DirectionR==DirectionlastR)
  {
    if(DirectionR)
      speedvalueR++;
    else
      speedvalueR--;
  }
  else
  {
    speedvalueR=0;
    if(DirectionR)
      speedvalueR++;
    else
      speedvalueR--;
  }
  DirectionlastR=DirectionR;
}

void drive()
{
  analogWrite(EA,outputL);
  analogWrite(EB,outputR); 
}
void forbac()
{
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);

  float cm=pulseIn(echopin,HIGH);
  cm=(int(cm*100.0))/100.0;
  if(cm>25)
    forward();
  else if(cm<20)
    back();
  else 
    stopfb();
}
void forward()
{
  digitalWrite(pinLa,HIGH);
  digitalWrite(pinLb,LOW);
  digitalWrite(pinRa,HIGH);
  digitalWrite(pinRb,LOW);
  DirectionL=true;
  DirectionR=true;
  drive();
}

void back()
{
  digitalWrite(pinLa,LOW);
  digitalWrite(pinLb,HIGH);
  digitalWrite(pinRa,LOW);
  digitalWrite(pinRb,HIGH);
  DirectionL=false;
  DirectionR=false;
  drive();
}

void left()
{
  digitalWrite(pinLa,LOW);
  digitalWrite(pinLb,HIGH);
  digitalWrite(pinRa,HIGH);
  digitalWrite(pinRb,LOW);
  DirectionL=false;
  DirectionR=true;
  drive();
}

void right()
{
  digitalWrite(pinLa,HIGH);
  digitalWrite(pinLb,LOW);
  digitalWrite(pinRa,LOW);
  digitalWrite(pinRb,HIGH);
  DirectionL=true;
  DirectionR=false;
  drive();
}

void stopfb()
{
  analogWrite(EA,LOW);
  analogWrite(EB,LOW);
}


















