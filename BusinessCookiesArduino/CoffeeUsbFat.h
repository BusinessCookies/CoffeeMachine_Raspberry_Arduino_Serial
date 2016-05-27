#ifndef COFFEEUSBFAT_H
#define COFFEEUSBFAT_H

#include <Arduino.h> 
#include <string.h> 
#include <SPI.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <UsbFat.h>
#include <masstorage.h>
#include <Wire.h>

#include "defines.h"

// Variables Usb Flash Device ************************************************

// RCTM variable
static tmElements_t tm;

// USB host objects.
static USB usb;
static BulkOnly bulk(&usb);

// File system.
static UsbFat key(&bulk);

// Files to read/write in.
static File file_admin;
static File file_date;
static File file_minmoney;
static File file_normcoffee;
static File file_expcoffee;

// Functions Usb Flash Device ************************************************
boolean setup_usbdrive();
boolean setup_RTCM();
boolean setup_serial();
boolean update_DB();
void serial2Flush();
int csv_getNumberLine(File file);
int csv_getNumberColumn(File file);
int csv_findValue(File file, int number_column, String value );
String csv_getValue(File file, int number_column, int pos );
int csv_isIdAdmin(String rfid);
int csv_isCodeAdmin(String code);
void csv_writeChoice(String rfid,int choice);
String csv_itoa10(int toConvert, int leng);

int get_csv_beans(String rfid);
int get_csv_water(String rfid);
void set_csv_beans(String rfid, int beans);
void set_csv_water(String rfid,int water);

String csv_getUsersMoney(String rfid);
String csv_getExpCoffee();
String csv_getNormCoffee();
String csv_getMinMoney();
String csv_retrieveRfid(String code);


int get_int_datetime(char type);
void set_int_date(int datY, int datM, int datD);
void set_int_time(int dath, int datm, int dats);
#endif
