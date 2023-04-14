#ifndef DAD_FFT_
#define DAD_FFT_

#include <Arduino.h>
extern "C"{
  #include <arm_math.h>
}

#define SAMPLE_LENGTH         512
#define MAX_16_BIT            65536 - 1
#define IFFT_FLAG             0
#define SCALING_FACTOR        32*1/32768

//#define SKELETON_DEBUG_MODE

class DAD_FFT_App{
  public:
    DAD_FFT_App();
    
    // lol this time for sure
    void FFT_Calculate(float inData[SAMPLE_LENGTH], uint8_t outData[SAMPLE_LENGTH]);

    void FFT_Calculate(float inData[SAMPLE_LENGTH], float outData[SAMPLE_LENGTH/2]);

    void collectData(float sample[SAMPLE_LENGTH]);

  private:
    void FFT_Setup_Hann_Window (float hann[SAMPLE_LENGTH]);
    
    float hann[SAMPLE_LENGTH];
    arm_status status;
};

#endif