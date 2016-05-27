#ifndef COFFEERFID_H
#define COFFEERFID_H

#include <Arduino.h> 
#include <string.h> 
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#include "defines.h"

// Variables rfid ************************************************
static Adafruit_NFCShield_I2C nfc(IRQ, RESET);
static boolean success;
static uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
static uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
static char buffer [33];
  
// Functions rfid ************************************************
boolean setup_nfc();
String rfid_getId();
String rfid_convertID();
String rfid_itoa16(int toConvert, int leng);

#endif

