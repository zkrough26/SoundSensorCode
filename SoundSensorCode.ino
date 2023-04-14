#include "SparkFun_SinglePairEthernet.h"
#include "arduinoFFT.h"

#define ChipSelPin CS
#define InterruptPin G1
#define ResetPin G0

SinglePairEthernet adin1110;
arduinoFFT FFT;

byte deviceMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0x03};
byte destinationMAC[6] = {0x00, 0xE0, 0x22, 0xFE, 0xDA, 0xCA};

const uint16_t samples = 1024;
const double samplingFrequency = 10000;
double vReal[samples];
double vImag[samples];

unsigned long lastBlink = 0;

void setup() 
{
    pinMode(A0, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    analogReadResolution(8);
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
    collectData();
    calculateFFT();
    sendSensData();

    unsigned long now = millis();
    if((now - lastBlink >= 1000) && (adin1110.getLinkStatus()))
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); 
        lastBlink = now;
    }
}

void collectData(){
  for(int i = 0; i < samples; i++)
  {
      vImag[i] = 0;
  }
  long int delay1, delay2, delay3, delay4; 
  for(int i = 0; i < samples; i++){
    vReal[i] = analogRead(A0);    
    delayMicroseconds(19);
    delay1++; delay2++; delay3++; delay4++;
  }
}

void calculateFFT()
{
  FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency); /* Create FFT object */
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(); /* Compute magnitudes */
}

void sendSensData()
{
  if(adin1110.getLinkStatus())
  {
    adin1110.sendData((byte*)vReal, sizeof(vReal), destinationMAC);
  }
  delay(1000);
}
