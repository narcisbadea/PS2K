#include <Servo.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SPI.h>

int topLeft;
int topRight;
int downLeft;
int downRight;

Servo leftRight;
Servo topDown;

int servoLR=90;
int servoTD=90;

long previousTime=0;
long previousTimeV=0;

Adafruit_INA219 ina219;
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float power_mW = 0;
float loadvoltage = 0;

typedef struct waypoint{
  int LR;
  int TD;
  String hour;
}waypoint;



void setup() {
  ina219.begin();
  leftRight.attach(10);
  topDown.attach(9);
  leftRight.write(servoLR);
  topDown.write(servoTD);
  Serial.begin(9600);
  previousTime = millis();
  
}

void loop() {

  if (millis() - previousTimeV > 1000){
    previousTimeV = millis();
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    Serial.println("shuntvoltage: " + (String)shuntvoltage);
    Serial.println("busvoltage: " + (String)busvoltage);
    Serial.println("current_mA: " + (String)current_mA);
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    Serial.println("loadvoltage: " + (String)loadvoltage);
    Serial.println("power_mW: " + (String)power_mW);
    Serial.println();
    
  }
  if(millis() - previousTime > 100){
    previousTime = millis();
    
    topLeft = analogRead(A0);
    topRight = analogRead(A1);
    downLeft = analogRead(A2);
    downRight = analogRead(A3);
  
    if(topLeft>topRight && downLeft>downRight){
      servoLR++;
    }
    if(topRight>topLeft && downRight>downLeft){
      servoLR--;
    }
    if(topLeft>downLeft && topRight>downRight){
      servoTD--;
    }
    if(downLeft>topLeft && downRight>topRight){
      servoTD++;
    }
    if(servoLR > 180){
      servoLR = 180;
    }
    if(servoLR < 0){
      servoLR = 0;
    }

    if(servoTD < 90)
    {
      servoTD = 90;
    }
    if(servoTD > 140){
      servoTD = 140;
    }

    leftRight.write(servoLR);
    topDown.write(servoTD);
  }
}
