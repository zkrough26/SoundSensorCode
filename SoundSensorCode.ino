#include "SparkFun_SinglePairEthernet.h"
#include "DAD_FFT_App.h"

#define ChipSelPin CS
#define InterruptPin G1
#define ResetPin G0

SinglePairEthernet adin1110;
DAD_FFT_App fftApp;

byte deviceMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0x03};
byte destinationMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0xCA};

float dataBuffer[512];
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
    collectData(dataBuffer);
    calculateFFT();
    sendSensData();

    unsigned long now = millis();
    if((now - lastBlink >= 1000) && (adin1110.getLinkStatus()))
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
        lastBlink = now;
    }
}

void collectData(float sample[SAMPLE_LENGTH]){
  long int delay1, delay2, delay3, delay4; 
  for(int i = 0; i < SAMPLE_LENGTH; i++){
    sample[i] = analogRead(A0);    
    delayMicroseconds(19);
    delay1++; delay2++; delay3++; delay4++; // delayMicroseconds only has so much resolution. Incrementing these arificially brings us closer to the targetted delay
  }

  // Remove DC offset
  for(int i = 0; i < SAMPLE_LENGTH; i++){
    sample[i] = sample[i] - 32857.36;    
  }
}

void calculateFFT()
{
  fftApp.FFT_Calculate(dataBuffer, outputBuffer);
  memset(dataBuffer, 1, sizeof(dataBuffer));
}

void sendSensData()
{
  if(adin1110.getLinkStatus())
  {
    adin1110.sendData(outputBuffer, sizeof(outputBuffer), destinationMAC);
  }
  memset(outputBuffer, 0, sizeof(outputBuffer));
  counter = 0;
  delay(1000);
}
