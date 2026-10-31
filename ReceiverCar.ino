#include <RH_ASK.h>
#include <SPI.h>
#include "Arduino.h"

RH_ASK driver(2000, 11);

//wheels
int forward = 4;
int backward = 5;

//steering
int left = 7;
int right = 6;

//LEDs
int offLed = 2;
int onLed = 3;

int forwardButton = 9;
int prevForward = false;
int backwardButton = 13;

bool forwardBool = false;
bool backwardBool = false;
bool leftBool = false;
bool rightBool = false;

bool forwardState = false;
int forwardDelay = 49;
int forwardCount = 0;
bool leftState = false;
bool prevLeftState = false;
int leftDelay = 70;
int leftCount = 0;


bool stop = false;
bool prevStop = false;
int linger = 0;

bool off = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!driver.init())
         Serial.println("init failed");

  pinMode(forward, OUTPUT);
  pinMode(backward, OUTPUT);
  
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);

  pinMode(offLed, OUTPUT);
  pinMode(onLed, OUTPUT);



  pinMode(forwardButton, INPUT_PULLUP);
  pinMode(backwardButton, INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // uint8_t buf = 1000;
  if(linger < 0) {
    forwardBool = false;
    backwardBool = false;
    leftBool = false;
    rightBool = false;
  }



  uint8_t buf[1];
  uint8_t bufSize = sizeof(buf);
  linger--;

  if(driver.recv(buf, &bufSize)) {
    // Serial.println("msg:");
    const char *bufChar = (char*)buf;
    int bufInt = *bufChar;
    Serial.println(bufInt);
    prevStop = stop && linger > 0;

    linger = 23;
    stop = (bufInt % 11 == 0);
    if(prevStop == false && stop == true) {
      off = !off;
    }
    forwardBool = ((bufInt % 5 == 0) && !(bufInt % 7 == 0));
    backwardBool = ((bufInt % 7 == 0) && !(bufInt % 5 == 0));
    leftBool = ((bufInt % 2 == 0) && !(bufInt % 3 == 0));
    rightBool = ((bufInt % 3 == 0) && !(bufInt % 2 == 0));
    if(bufInt != 1) {
      leftState = false;
      forwardState = false;
      leftCount = 0;
      forwardCount = 0;
    }
  } 
  else if(!off) {
    if(!digitalRead(backwardButton)) {
      forwardState = false;
      forwardCount = 0;
      if(!leftState) {
        leftBool = true;
        leftCount++;
        leftState = leftCount >= leftDelay;
      } else {
        backwardBool = true;
      }
    } else if(!digitalRead(forwardButton)) {
      if(leftState) {
        prevLeftState = true;
        leftState = false;
        rightBool = true;
        leftCount = 0;
        forwardCount = 1;
      } else if(prevLeftState && !forwardState) {
        rightBool = true;
        forwardCount++;
        forwardState = forwardCount >= forwardDelay;
      } else if(!leftState && !forwardState) {
        leftBool = true;
        leftCount++;
        leftState = leftCount >= leftDelay;
      } else if(forwardState) {
        forwardBool = true;
      }
    }
  }
  Serial.println(off);
  if(forwardBool) Serial.println("Forward");
    if(backwardBool) Serial.println("Backward");
  if(rightBool) Serial.println("right");
  if(leftBool) Serial.println("left");

  digitalWrite(onLed, true);
  digitalWrite(offLed, off);
  digitalWrite(forward, forwardBool);
  digitalWrite(backward, backwardBool);
  digitalWrite(left, leftBool);
  digitalWrite(right, rightBool);
}






