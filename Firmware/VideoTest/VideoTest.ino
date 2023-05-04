/****************************************************************************************************************************
Uses the HW PWM Module to seamlessly generate 2 pulse signals, one is the clock, one the ST pulse.
one is 2548x longer than the other. So all the line can be whitnessed first on the scope.
Needed to tie pin 9 to 10 since the different FRQs can only be handled by separate HW PWM parts of the perifery
*****************************************************************************************************************************/
//level 3 ist für serial Ausgabe von was grade passiert
#define _PWM_LOGLEVEL_        3
#include "RP2040_PWM.h"

//creates pwm instance
//RP2040_PWM* PWM_Instance;
RP2040_PWM* PWM_ClkInstance;
RP2040_PWM* PWM_StInstance;

float frequency2 = 2000; // Um das zu erreichen, braucht Oszillator 125MHz langsam zu sein.
float frequency = 5000000;

float dutyCycle2 = 20;
float dutyCycle = 50;

#define pinToUse      25 
#define ClkPin        10
#define StPin         8


void setup()
{

  PWM_StInstance = new RP2040_PWM(StPin, frequency2, dutyCycle2);
  PWM_ClkInstance = new RP2040_PWM(ClkPin, frequency, dutyCycle);

  PWM_StInstance->setPWM();
  PWM_ClkInstance->setPWM();
  // initialize serial:
  Serial.begin(115200);
  while (Serial.available()) 
    Serial.read(); //just to flush first the buffer
}

void loop()
{
  // print the string when a newline arrives:
  while (Serial.available()) {

    frequency = Serial.parseFloat();
    dutyCycle = Serial.parseFloat();
    frequency2 = Serial.parseFloat();
    dutyCycle2 = Serial.parseFloat();
  
    Serial.print(frequency);
    Serial.print(' ');
    Serial.print(dutyCycle);
    Serial.print(' ');
    Serial.print(frequency2);
    Serial.print(' ');
    Serial.println(dutyCycle2);
    
    PWM_ClkInstance->setPWM(ClkPin, frequency, dutyCycle);
    PWM_StInstance->setPWM(StPin, frequency2, dutyCycle2);
    
    while(Serial.available()) {
      Serial.read();
      };


    
   } 

}

/*

void setup1()
{
   //assigns pin 25 (built in LED), with frequency of 20 KHz and a duty cycle of 0%
  PWM_Instance = new RP2040_PWM(pinToUse, 200000, 40);
  PWM_Instance->setPWM();
  
}

void loop1()
{
  
  PWM_Instance->setPWM(pinToUse, 200000, 10);
  delay(1000);
  PWM_Instance->setPWM(pinToUse, 200000, 90);
  delay(1000);
  
}
*/