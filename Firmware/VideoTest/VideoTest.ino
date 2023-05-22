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

float STfrequency = 2000; // Um das zu erreichen, braucht Oszillator 125MHz langsam zu sein.
float Clkfrequency = 5000000;

float STDutyCycle = 10;
float ClkDutyCycle = 50;

#define pinToUse      25 
#define ClkPin        10
#define StPin         8
#define debugPin      19

#define ADCIn1        5
#define ADCIn2        4
#define ADCIn3        3
#define ADCIn4        2



int ADCSum;
int ClkCounter = 0;
boolean STStatus;
boolean STStatusOld = false;
boolean ClkStatus;
boolean ClkStatusOld = false;
String ADCResults = {"Testpattern. This should be overwritten after first capture."};

void setup()
{

  pinMode(ADCIn1,INPUT);
  pinMode(ADCIn2,INPUT);
  pinMode(ADCIn3,INPUT);
  pinMode(ADCIn4,INPUT);
  pinMode(pinToUse,OUTPUT);
  pinMode(debugPin, OUTPUT);
  


}

void loop()
{

  ClkStatus = digitalRead(ClkPin);
  STStatus = digitalRead(StPin);
//  digitalWrite(pinToUse,STStatus);
//  digitalWrite(debugPin,ClkStatus);
  if ((STStatus == false ) && (STStatusOld == true )){
  digitalWrite(pinToUse,HIGH);}
  else
  digitalWrite(pinToUse,LOW);

/*    Serial.print(STStatus);
    Serial.print(STStatusOld);
    Serial.print(ClkStatus);
    Serial.print(ClkStatusOld);
    Serial.println(ClkCounter);

 //   Serial.println(ADCResults);
    ClkCounter = 0;
  }
*/  
  if ((ClkStatusOld == false) && (ClkStatus == true)){
  digitalWrite(debugPin,HIGH);}
  else
  digitalWrite(debugPin,LOW);
/*

    if ((ClkCounter > 50)&&(ClkCounter < 2547)){
      ADCSum =  digitalRead(ADCIn1)+digitalRead(ADCIn2)+digitalRead(ADCIn3)+digitalRead(ADCIn4)+0x30;
      ADCResults[ClkCounter-50] = ADCSum;
    }
    
    ClkCounter++;
    if (ClkCounter >= 2546)
     Serial.print(ClkCounter);
  }
*/
  ClkStatusOld = ClkStatus;
  STStatusOld = STStatus;
}



void setup1()
{
  PWM_StInstance = new RP2040_PWM(StPin, STfrequency, STDutyCycle);
  PWM_ClkInstance = new RP2040_PWM(ClkPin, Clkfrequency, ClkDutyCycle);

  PWM_StInstance->setPWM();
  PWM_ClkInstance->setPWM();
  // initialize serial:
  Serial.begin(115200);
}

void loop1()
{
  
  // print the string when a newline arrives:
  while (Serial.available()) {

    Clkfrequency = Serial.parseFloat();
    ClkDutyCycle = Serial.parseFloat();
    STfrequency = Serial.parseFloat();
    STDutyCycle = Serial.parseFloat();
  
    Serial.print(Clkfrequency);
    Serial.print(' ');
    Serial.print(ClkDutyCycle);
    Serial.print(' ');
    Serial.print(STfrequency);
    Serial.print(' ');
    Serial.println(STDutyCycle);
    
    PWM_ClkInstance->setPWM(ClkPin, Clkfrequency, ClkDutyCycle);
    PWM_StInstance->setPWM(StPin, STfrequency, STDutyCycle);
    
    while(Serial.available()) {
      Serial.read();
      };
   } 

}
