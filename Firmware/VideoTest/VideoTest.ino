/****************************************************************************************************************************
  basic_pwm.ino
  For RP2040 boards
  Written by Dr. Benjamin Bird
  A basic example to get you up and running.
  Library by Khoi Hoang https://github.com/khoih-prog/RP2040_PWM
  Licensed under MIT license
  The RP2040 PWM block has 8 identical slices. Each slice can drive two PWM output signals, or measure the frequency
  or duty cycle of an input signal. This gives a total of up to 16 controllable PWM outputs. All 30 GPIO pins can be driven
  by the PWM block
*****************************************************************************************************************************/

#define _PWM_LOGLEVEL_        3
#include "RP2040_PWM.h"

//creates pwm instance
RP2040_PWM* PWM_Instance;
RP2040_PWM* PWM_ClkInstance;
RP2040_PWM* PWM_StInstance;

float frequency;
float dutyCycle;

#define pinToUse      25
#define ClkPin        10
#define StPin         8


void setup()
{
  //assigns pin 25 (built in LED), with frequency of 20 KHz and a duty cycle of 0%
  PWM_Instance = new RP2040_PWM(pinToUse, 200000, 40);
  PWM_StInstance = new RP2040_PWM(StPin, 200, 10);
  PWM_ClkInstance = new RP2040_PWM(ClkPin, 509600, 50);
  PWM_Instance->setPWM();
  PWM_StInstance->setPWM();
  PWM_ClkInstance->setPWM();
}

void loop()
{
  delay(1000);
  frequency = 200;
  dutyCycle = 90;

  PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
  PWM_ClkInstance->setPWM(ClkPin, 509600, 49);
  PWM_StInstance->setPWM(StPin, frequency, 10);

  delay(1000);
  dutyCycle = 10;

  PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
  PWM_ClkInstance->setPWM(ClkPin, 509600, 51);
  PWM_StInstance->setPWM(StPin, frequency, 20);
  
}
