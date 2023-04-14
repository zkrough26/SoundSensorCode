#include "DAD_FFT_App.h"

DAD_FFT_App::DAD_FFT_App() {
  FFT_Setup_Hann_Window(hann);
}

// Calculates FFT
void DAD_FFT_App::FFT_Calculate(float inData[SAMPLE_LENGTH], uint8_t outData[SAMPLE_LENGTH]) {
  float tempOut[SAMPLE_LENGTH/2];

  // float debug[SAMPLE_LENGTH];
  // for(int i = 0; i < SAMPLE_LENGTH; i++){
  //   debug[i] = inData[i];        
  // }

  this->FFT_Calculate(inData, tempOut);

  // tempOut is of size SAMPLE_LENGTH/2. In order to keep the rest of the code the same, output is SAMPLE_LENGTH
  for(int i = 0; i < SAMPLE_LENGTH/2; i++){
    if(tempOut[i] * SCALING_FACTOR >= 255)
      tempOut[i] = 254;    
    else
      tempOut[i] = tempOut[i] * SCALING_FACTOR;
      
    if((uint8_t)tempOut[i] == 255)
      tempOut[i] = 254;    
    outData[i*2]    = (uint8_t) tempOut[i];
    outData[i*2+1]  = (uint8_t) tempOut[i];

    // outData[i*2]    = (uint8_t) debug[i];
    // outData[i*2+1]  = (uint8_t) debug[i] / 256;
  }
}


// Calculates FFT
void DAD_FFT_App::FFT_Calculate(float32_t inData[SAMPLE_LENGTH], float32_t outData[SAMPLE_LENGTH/2]) {
  int i;
  float32_t data_buff[SAMPLE_LENGTH];

  // Pass data through window
  for (i = 0; i < SAMPLE_LENGTH; i++) {
    inData[i] = (float32_t)(hann[i] * inData[i]);
  }
  // Init RFFT
  arm_rfft_fast_instance_f32 instance;
  status = arm_rfft_fast_init_f32(&instance, SAMPLE_LENGTH);
  // Compute FFT
   arm_rfft_fast_f32(&instance, inData, data_buff, IFFT_FLAG);
  // Calculate magnitude
  for (i = 0; i < SAMPLE_LENGTH; i += 2) {
    outData[i/2] =
      sqrtf((data_buff[i] * data_buff[i]) + (data_buff[i + 1] * data_buff[i + 1]));
  }
}

void DAD_FFT_App::collectData(float sample[SAMPLE_LENGTH]){
  uint16_t index = 0;
  while(index  < SAMPLE_LENGTH){
    delayMicroseconds(100);
    sample[index] = analogRead(A0);    
    index++;
  }
}

// Populates hann window to mitigate error from non-integer # of samples
void DAD_FFT_App::FFT_Setup_Hann_Window(float hann[SAMPLE_LENGTH]) {
  // Initialize Hann Window
  int n;
  for (n = 0; n < SAMPLE_LENGTH; n++) {
    hann[n] = 0.5f - 0.5f * cosf((2 * PI * n) / (SAMPLE_LENGTH - 1));
  }
}