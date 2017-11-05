#include <PID_v1.h>

#define pinA A1
#define INTA 2
#define INTB 3

const int EA=9;
const int EB=10;
const int pinLa=7;
const int pinLb=8;
const int pinRa=12;
const int pinRb=13;

double SetpointL, SetpointR;
double Kp=0.6, Ki=5, Kd=0;
double outputL, outputR;
double inputL, inputR, speedvalueL, speedvalueR;
bool DirectionL, DirectionR;
bool DirectionlastL, DirectionlastR;
PID pidL(&inputL,&outputL,&SetpointL,Kp,Ki,Kd,DIRECT);
PID pidR(&inputR,&outputR,&SetpointR,Kp,Ki,Kd,DIRECT);

void setup() {
  pinMode(INTA,INPUT);
  pinMode(INTB,INPUT);

  initleftwheel();
  initrightwheel();
  
  attachInterrupt(0,wheelspeedL,CHANGE);
  attachInterrupt(1,wheelspeedR,CHANGE);

  Serial.begin(9600);
}

void loop() {
  char key[2];
  if(Serial.available()>0)
  {   
      Serial.readBytes(key,sizeof(key)); 
  }
  switch(key[0])
  {
    case 'L':
      left();
      break;
    case 'R':
      right();
      break;
    default:
      break; 
  }

  switch(key[1])
  {
    case 'l':
      SetpointL=000;
      SetpointR=000;
      break;
    case 'm':
      SetpointL=000;
      SetpointR=000;
      break;
    case 'h':
      SetpointL=000;
      SetpointR=000;
      break;
    default:
      break;
  }
  inputL=abs(speedvalueL);
  inputR=abs(speedvalueR);

  if(pidL.Compute() && pidR.Compute())
  {
    speedvalueL=speedvalueR=0;
  }
}

void initleftwheel()
{
  pinMode(EA,OUTPUT);
  pinMode(pinLa,OUTPUT);
  pinMode(pinLb,OUTPUT);
  pidL.SetMode(AUTOMATIC);
  pidL.SetSampleTime(100);
}

void initrightwheel()
{
  pinMode(EB,OUTPUT);
  pinMode(pinRa,OUTPUT);
  pinMode(pinRb,OUTPUT);
  pidR.SetMode(AUTOMATIC);
  pidR.SetSampleTime(100);
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

void forward()
{
  digitalWrite(pinLa,HIGH);
  digitalWrite(pinLb,LOW);
  digitalWrite(pinRa,HIGH);
  digitalWrite(pinRb,LOW);
  DirectionL=true;
  DirectionR=true;
  analogWrite(EA,outputL);
  analogWrite(EB,outputR);  
}

void back()
{
  digitalWrite(pinLa,LOW);
  digitalWrite(pinLb,HIGH);
  digitalWrite(pinRa,LOW);
  digitalWrite(pinRb,HIGH);
  DirectionL=false;
  DirectionR=false;
  analogWrite(EA,outputL);
  analogWrite(EB,outputR);
}

void left()
{
  digitalWrite(pinLa,LOW);
  digitalWrite(pinLb,HIGH);
  digitalWrite(pinRa,HIGH);
  digitalWrite(pinRb,LOW);
  DirectionL=false;
  DirectionR=true;
  analogWrite(EA,outputL);
  analogWrite(EB,outputR);
}

void right()
{
  digitalWrite(pinLa,HIGH);
  digitalWrite(pinLb,LOW);
  digitalWrite(pinRa,LOW);
  digitalWrite(pinRb,HIGH);
  DirectionL=true;
  DirectionR=false;
  analogWrite(EA,outputL);
  analogWrite(EB,outputR);
}

void stop()
{
  analogWrite(EA,LOW);
  analogWrite(EB,LOW);
}



















