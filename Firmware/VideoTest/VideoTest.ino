/****************************************************************************************************************************
Uses the HW PWM Module to seamlessly generate 2 pulse signals, one is the clock, one the ST pulse.
one is 2548x longer than the other. So all the line can be whitnessed first on the scope.
Needed to tie pin 9 to 10 since the different FRQs can only be handled by separate HW PWM parts of the perifery
*****************************************************************************************************************************/

#define _PWM_LOGLEVEL_        3
#include "RP2040_PWM.h"

//creates pwm instance
RP2040_PWM* PWM_Instance;
RP2040_PWM* PWM_ClkInstance;
RP2040_PWM* PWM_StInstance;

float frequency = 2000; // Um das zu erreichen, braucht Oszillator 125MHz langsam zu sein.
float frequency2 = 5200000;

float dutyCycle = 50;
float dutyCycle2 = 10;

#define pinToUse      25 
#define ClkPin        10
#define StPin         8

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


void setup()
{
  //assigns pin 25 (built in LED), with frequency of 20 KHz and a duty cycle of 0%
  PWM_Instance = new RP2040_PWM(pinToUse, 200000, 40);
  PWM_StInstance = new RP2040_PWM(StPin, frequency, 10);
  PWM_ClkInstance = new RP2040_PWM(ClkPin, frequency2, dutyCycle);
  PWM_Instance->setPWM();
  PWM_StInstance->setPWM();
  PWM_ClkInstance->setPWM();
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);  
}

void loop()
{
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
   } 

  PWM_Instance->setPWM(pinToUse, 200000, 90);
  PWM_ClkInstance->setPWM(ClkPin, frequency2, dutyCycle2);
  PWM_StInstance->setPWM(StPin, frequency, dutyCycle);

}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
