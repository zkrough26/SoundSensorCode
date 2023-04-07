#include "DAD_FFT_App.h"

DAD_FFT_App::DAD_FFT_App() {
  FFT_Setup_Hann_Window(hann);
}

// Calculates FFT
arm_status DAD_FFT_App::FFT_Calculate(float inData[SAMPLE_LENGTH], uint8_t outData[SAMPLE_LENGTH]) {
  int i = 0;
  float data_buff[SAMPLE_LENGTH * 2];

  // Set up window
  for (i = 0; i < SAMPLE_LENGTH; i++) {
    inData[i] = (float)(hann[i] * inData[i]);
  }
  // Init RFFT
  arm_rfft_instance_f32 instance;
  arm_cfft_radix4_instance_f32 radixInst;
  status = arm_rfft_init_f32(&instance, &radixInst, SAMPLE_LENGTH, IFFT_FLAG, DO_BIT_REVERSE);
  // Compute FFT
  arm_rfft_f32(&instance, inData, data_buff);
  // Calculate magnitude
  for (i = 0; i < 2 * SAMPLE_LENGTH; i += 2) {
    outData[i / 2] =
      (uint8_t)(sqrtf((data_buff[i] * data_buff[i]) + (data_buff[i + 1] * data_buff[i + 1])));  // Max is 256
  }
  return status;
}


// Populates hann window to mitigate error from non-integer # of samples
void DAD_FFT_App::FFT_Setup_Hann_Window(float hann[SAMPLE_LENGTH]) {
  // Initialize Hann Window
  int n;
  for (n = 0; n < SAMPLE_LENGTH; n++) {
    hann[n] = 0.5f - 0.5f * cosf((2 * PI * n) / (SAMPLE_LENGTH - 1));
  }
}
