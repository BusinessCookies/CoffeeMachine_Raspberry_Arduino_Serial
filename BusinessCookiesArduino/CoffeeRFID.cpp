#include "CoffeeRFID.h"

boolean setup_nfc()
{
  #ifdef COFFEERFIDDEBUG
  Serial.println();
  Serial.println("--------------------");
  Serial.println(F("RFID Setup"));
  Serial.println("--------------------");
  #endif
  
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    #ifdef COFFEERFIDDEBUG
    Serial.print(F("Didn't find PN53x board"));
    #endif
    return false;
  }
  
  // Got ok data, print it out!
  #ifdef COFFEERFIDDEBUG
  Serial.print(F("Found chip PN5")); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print(F("Firmware ver. ")); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  #endif
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  return true;
}

String rfid_itoa16(int toConvert, int leng)
{
  char converted[RFID_STRING_BUFFER_SIZE];
  itoa(toConvert,converted,16);
  int diff = leng - String(converted).length();
  String returned = ""; 
  for(int i = 0; i < diff; i++)
  {
    returned += "0";
  } 
  returned += converted;
  return returned; 
}

String rfid_convertID()
{
  String rfid_string = ""; 
  
  for (uint8_t i=0; i < uidLength; i++) 
  {
    rfid_string += rfid_itoa16(uid[i],2);
  }
  rfid_string.toLowerCase(); 
  return rfid_string;  
}

String rfid_getId()
{

  // If an RFID card came afer last call, success = true, else if = false 
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success) {
    #ifdef COFFEERFIDDEBUG
    Serial.println(F("Found a card!"));
    Serial.print(F("UID Length: "));Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print(F("UID Value: "));
    
    Serial.println();
    Serial.println(F("HEX"));
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(F("-"));Serial.print(uid[i],HEX); 
    }
    Serial.println();
    #endif
    delay(1000);
    return rfid_convertID();
  }
  else
  {
    #ifdef COFFEERFIDDEBUG
    Serial.println(F("Time out waiting for a card"));
    #endif
    return RFID_NULL;
  }
  
}
