#ifndef DAD_FFT_
#define DAD_FFT_

#include <Arduino.h>
extern "C"{
  #include <arm_math.h>
  //#include <arm_const_structs.h>
}

#define SAMPLE_LENGTH         512
#define IFFT_FLAG             0
#define DO_BIT_REVERSE        1

class DAD_FFT_App{
  public:
    DAD_FFT_App();
    
    // lol this time for sure
    arm_status FFT_Calculate(float inData[SAMPLE_LENGTH], uint8_t outData[SAMPLE_LENGTH]);
    
  private:
    void FFT_Setup_Hann_Window (float hann[SAMPLE_LENGTH]);
    
    float hann[SAMPLE_LENGTH];
    arm_status status;
};

#endif