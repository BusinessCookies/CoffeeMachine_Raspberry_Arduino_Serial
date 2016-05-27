#ifndef COFFEETOUCHSCREEN_H
#define COFFEETOUCHSCREEN_H

#include <Arduino.h> 
#include <string.h> 

#include <Adafruit_GFX.h>   
#include <Adafruit_ILI9341.h>
#include "TouchScreen.h"

#include <SPI.h>

#include <UsbFat.h>
#include <masstorage.h>
#include <Wire.h>

#include "defines.h"
#include "picojpeg.h"

typedef unsigned char uint8;
typedef unsigned int uint;

// Variables Touch Screen ************************************************
static Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
static TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Files to read/write in.
static File bmpFile;
static File g_pInFile;
static uint g_nInFileSize;
static uint g_nInFileOfs;



// Strucure ************************************************
typedef struct Button Button;
struct Button
{
    int x;
    int y;
    int w;
    int h;
};

// Functions Touch Screen ************************************************
boolean isTouched(Button *button, TSPoint *p);
boolean setup_touchscreen();
boolean setup_sd();

void jpegDraw(char *pFilename); // must be jpeg 4:2:2 horizontal without progressivity
void bmpDraw(char *filename, uint8_t x, uint16_t y);
uint16_t read16(File &f);
uint32_t read32(File &f);

void rotateAndScaleTouch();
void tft_print(String toPrint, uint16_t x, uint16_t y, uint8_t t, uint16_t textColor, uint16_t bgColor);
void tft_println(String toPrint);

int touch_waitrfid();
int touch_nonadmin();
int touch_admin();  
int touch_mycoffeesetup();  
int touch_setup();
int touch_date1();     
int touch_date2();

void tft_notenough();
void tft_waitrfid();
void tft_thankyou();
void tft_nonadmin();
void tft_admin();
void tft_mycoffee();
void tft_othercoffee(); 
void tft_mycoffeesetup();
void tft_setup();
void tft_lock();
void tft_date1();
void tft_date2();

void tft_datetime_update(int dat1, int dat2, int dat3);
void tft_mycoffee_update(int beans, int water);
void tft_wait_update(int code_size);
void tft_select_update(String minmoney,String usersmoney,String normcoffee,String expcoffee);
#endif

