#ifndef COFFEEINTERFACE_H
#define COFFEEINTERFACE_H

#include <Arduino.h> 
#include <string.h> 

#include "defines.h"

// Variables rfid ************************************************
static boolean state_rota = false;
  
// Functions rfid ************************************************
void setup_interface();
void interface_othercoffee(int choice);
void pressSimul(int pinButton);
void rotaRightSimul();
void rotaLeftSimul();
void longPressSimulRP();

#endif

