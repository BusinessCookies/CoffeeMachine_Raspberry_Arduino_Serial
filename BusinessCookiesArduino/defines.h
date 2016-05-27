#ifndef DEFINES_H
#define DEFINES_H

// Uncomment to enable debug
#define COFFEETOUCHSCREENDEBUG
#define COFFEEUSBFATDEBUG
#define COFFEERFIDDEBUG
#define COFFEEDEBUG

//PIN Button for coffee interface
#define TIME_PRESS_BUTTON 1500
#define TIME_LONG_PRESS_BUTTON 5000

#define TIME_ROTA_DELAY 150

#define TIME_DELAY_THANK 2000

#define TIME_DELAY_MYCOFFEE1 500
#define TIME_DELAY_MYCOFFEE2 3000
#define AMOUNT_BEAN_SB 3
#define AMOUNT_WATER_SB 180


#define PIN_BUTTON_SS 31
#define PIN_BUTTON_DS 30
#define PIN_BUTTON_SB 33
#define PIN_BUTTON_DB 32
#define PIN_BUTTON_CC 34
#define PIN_BUTTON_FC 35
#define PIN_BUTTON_RP 24
#define PIN_BUTTON_RA 38
#define PIN_BUTTON_RB 39
#define PIN_BUTTON_ST 37
#define PIN_BUTTON_CL 36

#define BYTE_ACK 6
#define BYTE_END 0
#define BYTE_STA 1


// Time out for choice selection
#define CHOICE_TIMEOUT 5000

// Define the main states of the program 
#define STATE_NONE -1
#define STATE_WAITRFID 0
#define STATE_ADMIN 1
#define STATE_NONADMIN 2
#define STATE_MYCOFFEE 3
#define STATE_OTHERCOFFEE 4
#define STATE_MYCOFFEESETUP 5
#define STATE_SETUP 6
#define STATE_THANKYOU 7
#define STATE_LOCK 8
#define STATE_DATE1 9
#define STATE_DATE2 10
#define STATE_NOTENOUGH 11


// Default null RFID ID 
#define RFID_NULL ""
#define CODE_NULL ""

// Buffer size for stings and date for CoffeeUsbFat
#define BUFFER_SIZE 32
#define DATE_STRING_BUFFER_SIZE 5

// These are the four touchscreen analog pins
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 46   // can be a digital pin
#define XP 48   // can be a digital pin

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// Sreen dimension
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Block dimension
#define BORDER_SPACE 5
#define SPACE_NON_ADMIN 10
#define SPACE_ADMIN 8
#define HEIGHT_NON_ADMIN  110 // must be /2
#define WIDTH_NON_ADMIN 150 // must be /2
#define HEIGHT_ADMIN  112 // must be /2
#define WIDTH_ADMIN 98 // must be /2

// BUTTONS DIMMENSIONS
#define BUTTON_WIDTH_1 80
#define BUTTON_HEIGHT_1 30
#define BUTTON_WIDTH_2 132
#define BUTTON_HEIGHT_2 30
#define BUTTON_WIDTH_3 51
#define BUTTON_HEIGHT_3 51
#define BUTTON_WIDTH_4 60
#define BUTTON_HEIGHT_4 60
#define BUTTON_WIDTH_5 120
#define BUTTON_HEIGHT_5 60
#define BUTTON_WIDTH_6 96
#define BUTTON_HEIGHT_6 96
#define BUTTON_WIDTH_7 48
#define BUTTON_HEIGHT_7 48

// for bitmap
#define BUFFPIXEL 20

//Pressure calibration 
#define MINPRESSURE 2
#define MAXPRESSURE 1000

// The display uses hardware SPI
#define TFT_CS 49
#define TFT_DC 48
#define SD_CS 47

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield

// rfid string buffer size
#define RFID_STRING_BUFFER_SIZE 3

#endif
