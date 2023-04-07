#include "SparkFun_SinglePairEthernet.h"

#define ChipSelPin CS
#define InterruptPin G1
#define ResetPin G0

SinglePairEthernet adin1110;

byte deviceMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0x03};
byte destinationMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0xCA};

byte dataBuffer[512];
int counter;
unsigned long lastBlink = 0;

void setup() 
{
    pinMode(A0, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    analogReadResolution(8);
    delay(1000);
    if (!adin1110.begin(deviceMAC, LED_BUILTIN, InterruptPin, ResetPin, ChipSelPin)) 
    {
      while(1)
      {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
      }
    }
    while (adin1110.getLinkStatus() != true);
}

void loop() {   
    dataBuffer[counter] = analogRead(A0);
    counter++;
    if (counter >= 512)
    {
      sendSensData();
    }
    delay(4);

    unsigned long now = millis();
    if((now - lastBlink >= 1000) && (adin1110.getLinkStatus()))
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
        lastBlink = now;
    }
    if(!adin1110.getLinkStatus())
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
}

void sendSensData()
{
  if(adin1110.getLinkStatus())
  {
    adin1110.sendData(dataBuffer, sizeof(dataBuffer), destinationMAC);
  }
  memset(dataBuffer, 0, sizeof(dataBuffer));
  counter = 0;
}
