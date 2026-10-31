#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver(2000, 6, 14, 15);

void setup()
{
    Serial.begin(9600);	  // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    const char *msg = "Hello World!";
    Serial.println(driver.send((uint8_t *)msg, strlen(msg)));
    driver.waitPacketSent();
    delay(1000);
}