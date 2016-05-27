/*************************************************************************************************
 Main file for the coffee machine interface. Used with:
 - Arduino Mega ADK
 - Adafruit PN532
 - Adafruit ILI9341
 - 4GB Usb flash drive (FAT32)
 - Real-Time Clock DS1307
 ************************************************************************************************/

//#################################################################################################
//############################      Header          ###############################################
//#################################################################################################

// Librairies ************************************************************
#include <Arduino.h> 
#include <string.h> 


#include "FatLib.h"

#include <Adafruit_GFX.h>    
#include <Adafruit_ILI9341.h>
#include "TouchScreen.h"

#include <Wire.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_NFCShield_I2C.h>

#include <Time.h>
#include <DS1307RTC.h>

#include <UsbFat.h>
#include <masstorage.h>

// Project files *********************************************************
#include "CoffeeRFID.h"
#include "CoffeeTouchScreen.h"
#include "CoffeeUsbFat.h"
#include "CoffeeInterface.h"

// Others files **********************************************************
#include "defines.h"
#include "states.h"

#include "picojpeg.h"




//#################################################################################################
//############################      Setup          ################################################
//#################################################################################################
void setup() 
{
  Serial.begin(115200);
  #ifdef COFFEERFIDDEBUG
  Serial.print(F("Starting setup"));
  #endif
  
  //Setup INTERFACE
  setup_interface();
  
  // Setup TOUCHSCREEN 
  while(!setup_touchscreen())
  {
    #ifdef COFFEERFIDDEBUG
    Serial.print(F("Can't setup TOUCHSCREEN"));
    #endif
    delay(1000);
  }
  tft_println("Setup...");
  
  // Setup SD 
  while(!setup_sd())
  {
    #ifdef COFFEERFIDDEBUG
    Serial.print(F("Can't setup SD"));
    tft_println("Can't setup SD");
    #endif
    delay(1000);
  }
  tft_println("SD OK...");
  
  // Setup RTCM
  /*while(!setup_RTCM())
  {
    #ifdef COFFEERFIDDEBUG
    Serial.print(F("Can't setup TIMECLOCK"));
    #endif
    tft_println("Can't setup TIMECLOCK");
    delay(1000);
  }
  tft_println("TIMECLOCK OK...");*/
  
  // Setup USBFAT
  while(!setup_usbdrive())
  {
    #ifdef COFFEERFIDDEBUG
    Serial.print(F("Can't setup USBFAT"));
    #endif
    tft_println("Can't setup USBFAT");
    delay(1000);
  }
  tft_println("USB DRIVE OK...");
  
  // Setup RFID
  while(!setup_nfc())
  {
    #ifdef COFFEERFIDDEBUG
    Serial.print(F("Can't setup RFID"));
    #endif
    tft_println("Can't setup RFID");
    delay(1000);
  }
  tft_println("RFID READER OK...");
  
  while(!setup_serial())
  {
    #ifdef COFFEERFIDDEBUG
    Serial.print(F("Can't setup SERIAL"));
    #endif
    tft_println("Can't setup SERIAL");
    delay(1000);
  }
  tft_println("SERIAL OK...");
  
  #ifdef COFFEERFIDDEBUG
  Serial.print(F("Every output to LOW level"));
  #endif
  tft_println("Every output to LOW level");
  
  digitalWrite(PIN_BUTTON_SS, LOW);
  digitalWrite(PIN_BUTTON_DS, LOW);
  digitalWrite(PIN_BUTTON_SB, LOW);
  digitalWrite(PIN_BUTTON_DB, LOW);
  digitalWrite(PIN_BUTTON_CC, LOW);
  digitalWrite(PIN_BUTTON_FC, LOW);
  digitalWrite(PIN_BUTTON_RP, LOW);
  digitalWrite(PIN_BUTTON_RA, LOW);
  digitalWrite(PIN_BUTTON_RB, LOW); 
  digitalWrite(PIN_BUTTON_ST, LOW);
  digitalWrite(PIN_BUTTON_CL, LOW); 
  
  #ifdef COFFEERFIDDEBUG
  Serial.print(F("Setup ok, welcome !"));
  #endif
  tft_println("Setup ok, welcome !");
  delay(3000);
}

//#################################################################################################
//############################      Loop          #################################################
//#################################################################################################
void loop()
{
  switch ( current_state ) {
    case STATE_WAITRFID:
      state_waitrfid();
      break;
    case STATE_ADMIN:
      state_admin();
      break;
    case STATE_NONADMIN:
      state_nonadmin();
      break;
    case STATE_MYCOFFEE:
      state_mycoffee();
      break;
    case STATE_OTHERCOFFEE:
      state_othercoffee();
      break;
    case STATE_MYCOFFEESETUP:
      state_mycoffeesetup();
      break;
    case STATE_SETUP:
      state_setup();
      break;
    case STATE_THANKYOU:
      state_thankyou();
      break;
    case STATE_LOCK:
      state_lock();
      break;
    case STATE_DATE1:
      state_date1();
      break;
    case STATE_DATE2:
      state_date2();
      break;
    case STATE_NOTENOUGH:
      state_notenough();
      break;
    default:
      current_state = STATE_WAITRFID;
      break;
  }
  #ifdef COFFEERFIDDEBUG
  Serial.println(current_state);
  Serial.println(choice);
  #endif
}






