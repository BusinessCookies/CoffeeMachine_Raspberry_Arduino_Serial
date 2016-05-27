#include "CoffeeInterface.h"

void setup_interface()
{
  pinMode(PIN_BUTTON_SS, OUTPUT);
  pinMode(PIN_BUTTON_DS, OUTPUT);
  pinMode(PIN_BUTTON_SB, OUTPUT);
  pinMode(PIN_BUTTON_DB, OUTPUT);
  pinMode(PIN_BUTTON_CC, OUTPUT);
  pinMode(PIN_BUTTON_FC, OUTPUT);
  pinMode(PIN_BUTTON_RP, OUTPUT);
  pinMode(PIN_BUTTON_RA, OUTPUT);
  pinMode(PIN_BUTTON_RB, OUTPUT); 
  pinMode(PIN_BUTTON_ST, OUTPUT);
  pinMode(PIN_BUTTON_CL, OUTPUT); 
}

void longPressSimulRP()
{
  digitalWrite(PIN_BUTTON_RP, HIGH);   
  delay(TIME_LONG_PRESS_BUTTON);              
  digitalWrite(PIN_BUTTON_RP, LOW);   
}

void pressSimul(int pinButton)
{
  digitalWrite(pinButton, HIGH);   
  delay(TIME_PRESS_BUTTON);              
  digitalWrite(pinButton, LOW);    
}



void rotaRightSimul()
{
  state_rota = !state_rota;
  digitalWrite(PIN_BUTTON_RB, state_rota);   
  delay(TIME_ROTA_DELAY);              
  digitalWrite(PIN_BUTTON_RA, state_rota);
  delay(TIME_ROTA_DELAY); 
}

void rotaLeftSimul()
{
  state_rota = !state_rota;
  digitalWrite(PIN_BUTTON_RA, state_rota);   
  delay(TIME_ROTA_DELAY);              
  digitalWrite(PIN_BUTTON_RB, state_rota);
  delay(TIME_ROTA_DELAY); 
}


