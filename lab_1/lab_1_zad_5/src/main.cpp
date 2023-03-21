#include <Arduino.h>

#define pwmChannel 0
#define pwmRes 8
#define pwmFreq 5000
#define pwmPin 32

const int MAX_duty = (int)(pow(2, pwmRes) - 1);
int duty;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  ledcSetup(pwmChannel, pwmFreq, pwmRes);
  ledcAttachPin(pwmPin, pwmChannel);
}

void loop()
{
  // Increasing the LED brightness with PWM
  for (duty = 0; duty <= MAX_duty; duty++)
  {
    ledcWrite(pwmChannel, duty);
    delay(5);
  }
  // Decreasing the LED brightness with PWM
  for (duty = MAX_duty; duty >= 0; duty--)
  {
    ledcWrite(pwmChannel, duty);
    delay(5);
  }
}