#include <Servo.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SPI.h>
#include<stdio.h>
#include <string.h>

int topLeft;
int topRight;
int downLeft;
int downRight;

Servo leftRight;
Servo topDown;

int servoLR=90;
int servoTD=90;

long previousTime=0;


typedef struct waypoint1{
  int LR;
  int TD;
  String hour;
}waypoint1;

typedef struct waypoint2{
  int LR;
  int TD;
  String hour;
}waypoint2;
waypoint1 WP1;
waypoint2 WP2;


void setup() {
  leftRight.attach(10);
  topDown.attach(9);
  leftRight.write(servoLR);
  topDown.write(servoTD);
  Serial.begin(9600);
  previousTime = millis();
  
}

void loop() {
  
  char way[50]; //="100 110 100 110";
  if (Serial.available() > 0) {
    Serial.readBytes(way,50); //Read the serial data and store in var
    Serial.println(way); //Print data on Serial Monitor
  }
  
    ///////////////////////////////////
    char *token;
    const char *delimiter =" ";


    token = strtok(way, delimiter);
    //Serial.println(token);
    String auxlr1=(String)(token);
    WP1.LR=auxlr1.toInt();
    Serial.println(WP1.LR);

    token = strtok(NULL, delimiter);
    //Serial.println(token);
    String auxlr2=(String)(token);
    WP2.LR=auxlr2.toInt();
    Serial.println(WP2.LR);

    token = strtok(NULL, delimiter);
    //Serial.println(token);
    String auxtd1=(String)(token);
    WP1.TD=auxtd1.toInt();
    Serial.println(WP1.TD);

    token = strtok(NULL, delimiter);
    //Serial.println(token);
    String auxtd2=(String)(token);
    WP2.TD=auxtd2.toInt();
    Serial.println(WP2.TD);

    ////////////////////////////////////////////


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
    if(WP1.LR < WP2.LR)
      if(servoLR < WP1.LR || servoLR > WP2.LR)
      {
        servoLR= (WP1.LR + WP2.LR) / 2;
      }
    if(WP1.LR > WP2.LR)
      if(servoLR > WP1.LR || servoLR < WP2.LR)
      {
        servoLR= (WP1.LR + WP2.LR) / 2;
      }
    if(WP1.TD < WP2.TD)
      if(servoTD < WP1.TD || servoTD > WP2.TD)
      {
        servoTD= (WP1.TD + WP2.TD) / 2;
      }
    if(WP1.TD > WP2.TD)
      if(servoTD > WP1.TD || servoTD < WP2.TD)
      {
        servoTD= (WP1.TD + WP2.TD) / 2;
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
    if(servoTD > 154){
      servoTD = 154;
    };
    leftRight.write(servoLR);
    topDown.write(servoTD);
  }
}
