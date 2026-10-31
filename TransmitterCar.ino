#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 6, 14, 15);

int left = 8;
int right = 7;
int forward = 10;
int backward = 9;

int stop = 16;

int led = 5;

uint8_t txSignal = 1;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(forward, INPUT);
  pinMode(backward, INPUT_PULLUP);
  pinMode(stop, INPUT_PULLUP);

  pinMode(led, OUTPUT);

  if(driver.init()) {
    digitalWrite(led, HIGH);
  } else {
    Serial.print("Transmitter not initialized");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  txSignal = 1;
  if(!digitalRead(left)) {
    Serial.println("left");
    txSignal *= 2;
  }
  if(!digitalRead(right)) {
    Serial.println("right");
    txSignal *= 3;
  }
  if(digitalRead(forward)) {
    Serial.println("forward");
    txSignal *= 5;
  }
  if(!digitalRead(backward)) {
    Serial.println("back");
    txSignal *= 7;
  }
  if(!digitalRead(stop)) {
    Serial.println("stop");
    txSignal *= 11;
  }

  const char txSignalChar = static_cast<char>(txSignal);
  Serial.println(txSignalChar);
  Serial.println(static_cast<int>(txSignalChar));
  const char *txSignalAddr = &txSignalChar;
  Serial.println(static_cast<int>(*txSignalAddr));
  if(txSignalChar != 1) Serial.println(driver.send((uint8_t*)txSignalAddr, sizeof(*txSignalAddr)));
    driver.waitPacketSent();
  
}
