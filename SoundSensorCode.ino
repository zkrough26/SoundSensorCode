#include "SparkFun_SinglePairEthernet.h"
#include "DAT_FFT_App.h"

#define ChipSelPin CS
#define InterruptPin G1
#define ResetPin G0

SinglePairEthernet adin1110;
DAD_FFT_App fftApp;

byte deviceMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0x03};
byte destinationMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0xCA};

uint16_t dataBuffer[512];
byte outputBuffer[512];
int counter;
unsigned long lastBlink = 0;

void setup() 
{
    pinMode(A0, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    analogReadResolution(16);
    delay(500);
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
      calculateFFT();
      sendSensData();
    }
    delayMicroSeconds(100);

    unsigned long now = millis();
    if((now - lastBlink >= 1000) && (adin1110.getLinkStatus()))
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
        lastBlink = now;
    }
}

void calculateFFT()
{
  fftApp.FFT_Calculate((float *)dataBuffer, outputBuffer);
  memset(dataBuffer, 0, sizeof(dataBuffer))
}

void sendSensData()
{
  if(adin1110.getLinkStatus())
  {
    adin1110.sendData(outputBuffer, sizeof(outputBuffer), destinationMAC);
  }
  memset(outputBuffer, 0, sizeof(outputBuffer));
  counter = 0;
  delay(25000);
}
