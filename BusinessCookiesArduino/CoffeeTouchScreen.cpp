#include "CoffeeTouchScreen.h"

//**********************************************************************************************************
// SOME FUNCTIONS ******************************************************************************************
//**********************************************************************************************************

boolean setup_touchscreen()
{  
  #ifdef COFFEETOUCHSCREENDEBUG
  Serial.println();
  Serial.println(F("--------------------"));
  Serial.println(F("TouchScreen Setup"));
  Serial.println(F("--------------------"));
  #endif
  
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  return true;
}

boolean setup_sd()
{
    /* if (!SD.begin(SD_CS)) {return false;} */
  return true;
}

boolean isTouched(Button *button, TSPoint *p)
{
    if(p->y < (button->y + button->h) && p->y > button->y && p->x < (button->x + button->w) && p->x > button->x) return true; 
    return false;
}

void rotateAndScaleTouch(TSPoint *p)
{
  // Scale touch coordinates to the number of pixels of the screen 
  p->x = map(p->x, TS_MINX, TS_MAXX, 0, SCREEN_HEIGHT);
  p->y = map(p->y, TS_MINY, TS_MAXY, 0, SCREEN_WIDTH);
  // Apply the rotation of 90 degrees clockwise
  int buffx = p->x;
  p->x = p->y;
  p->y = SCREEN_HEIGHT - buffx;
  
  #ifdef COFFEETOUCHSCREENDEBUG
  Serial.println("Touch");
  Serial.print(F("X: "));
  Serial.println(p->x);
  Serial.print(F("Y: "));
  Serial.println(p->y);
  Serial.print(F("Z: "));
  Serial.println(p->z);
  #endif
}

void tft_print(String toPrint, uint16_t x, uint16_t y, uint8_t t, uint16_t textColor, uint16_t bgColor  )
{
   for(int i=0; i<toPrint.length(); i++ ) 
   {
     tft.drawChar(x + i*6*t, y,toPrint.charAt(i), textColor,bgColor, t);
   }
}

void tft_println(String toPrint)
{
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2); 
  tft.println(toPrint); 
}



//**********************************************************************************************************
// FOR EACH STATE TOUCH ************************************************************************************
//**********************************************************************************************************
int touch_waitrfid()
{
  // Retrieve a point  
  // 1 is power
  // 2 is cleaning
  // 3 is the rest of the screen => short menu press
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return -1;
  } else {
    // Scale from ~0->SPACE_NON_ADMIN00 to tft.width using the calibration #'s and rotates 
    rotateAndScaleTouch(&p);
    // Create the button for the small coffee choice 
    Button button0 = {76,165,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button1 = {23,8,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button2 = {76,8,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button3 = {131,8,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button4 = {23,60,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button5 = {76,60,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button6 = {131,60,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button7 = {23,113,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button8 = {76,113,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button9 = {131,113,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button10 = {131,165,BUTTON_WIDTH_7,BUTTON_HEIGHT_7};
    Button button11 = {246,9,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button12 = {246,76,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button13 = {246,147,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    
    // Check if touched
    if(isTouched(&button0, &p))
    {
      return 0;
    }
    if(isTouched(&button1, &p))
    {
      return 1;
    }
    if(isTouched(&button2, &p))
    {
      return 2;
    }
    if(isTouched(&button3, &p))
    {   
    return 3;
    }
    if(isTouched(&button4, &p))
    {
      return 4;
    }
    if(isTouched(&button5, &p))
    {
      return 5;
    }
    if(isTouched(&button6, &p))
    {   
    return 6;
    }
    if(isTouched(&button7, &p))
    {
      return 7;
    }
    if(isTouched(&button8, &p))
    {
      return 8;
    }
    if(isTouched(&button9, &p))
    {   
    return 9;
    }
    if(isTouched(&button10, &p))
    {
      return 10;
    }
    if(isTouched(&button11, &p))
    {
      return 11;
    }
    if(isTouched(&button12, &p))
    {
      return 12;
    }
    if(isTouched(&button13, &p))
    {   
    return 13;
    }
    return -1;
  }
}


int touch_nonadmin()
{
  // Retrieve a point  
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return 0;
  } else {
    // Scale from ~0->SPACE_NON_ADMIN00 to tft.width using the calibration #'s and rotates 
    rotateAndScaleTouch(&p);
    // Create the button for the small coffee choice 
    Button button1 = {8,20,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button2 = {112,20,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button3 = {216,20,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button4 = {8,124,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button5 = {112,124,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button6 = {216,124,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    // Check if touched
    if(isTouched(&button1, &p)){return 1;}
    if(isTouched(&button2, &p)){return 2;}
    if(isTouched(&button3, &p)){return 3;}
    if(isTouched(&button4, &p)){return 4;}
    if(isTouched(&button5, &p)){return 5;}
    if(isTouched(&button6, &p)){return 6;}
    // Case of screen touched, but not on a button
    return 0;
  }
}

int touch_admin()
{
   // Retrieve a point  
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return 0;
  } else {
    // Scale from ~0->SPACE_NON_ADMIN00 to tft.width using the calibration #'s and rotates 
    rotateAndScaleTouch(&p);
    // Create the button for the small coffee choice 
    Button button1 = {8,20,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button2 = {112,20,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button3 = {216,20,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button4 = {8,124,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button5 = {112,124,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    Button button6 = {216,124,BUTTON_WIDTH_6,BUTTON_HEIGHT_6};
    // Check if touched
    if(isTouched(&button1, &p)){return 1;}
    if(isTouched(&button2, &p)){return 2;}
    if(isTouched(&button3, &p)){return 3;}
    if(isTouched(&button4, &p)){return 4;}
    if(isTouched(&button5, &p)){return 5;}
    if(isTouched(&button6, &p)){return 6;}
    // Case of touched not on a button
    return 0;
  }
}
 
    
int touch_mycoffeesetup()
{
   // Retrieve a point  
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return 0;
  } else {
    // Scale from ~0->SPACE_NON_ADMIN00 to tft.width using the calibration #'s and rotates 
    rotateAndScaleTouch(&p);
    // Create the button for the small coffee choice 
    Button button1 = {261,8,BUTTON_WIDTH_3,BUTTON_HEIGHT_3};
    Button button2 = {8,92,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button3 = {248,92,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button4 = {8,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button5 = {248,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    // Check if touched
    if(isTouched(&button1, &p)){return 1;}
    if(isTouched(&button2, &p)){return 2;}
    if(isTouched(&button3, &p)){return 3;}
    if(isTouched(&button4, &p)){return 4;}
    if(isTouched(&button5, &p)){return 5;}
    // Case of touched not on a button
    return 0;
  }
}
     
int touch_setup()
{
   // Retrieve a point  
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return 0;
  } else {
    // Scale from ~0->SPACE_NON_ADMIN00 to tft.width using the calibration #'s and rotates 
    rotateAndScaleTouch(&p);
    
    // Create the button for the small coffee choice 
    Button button1 = {20,10,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button2 = {120,10,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button3 = {220,10,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button4 = {20,58,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button5 = {120,58,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button6 = {220,58,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button7 = {20,106,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button8 = {120,106,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button9 = {220,106,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button10 = {20,154,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button11 = {120,154,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button12 = {220,154,BUTTON_WIDTH_1,BUTTON_HEIGHT_1};
    Button button13 = {20,202,BUTTON_WIDTH_2,BUTTON_HEIGHT_2};
    Button button14 = {168,202,BUTTON_WIDTH_2,BUTTON_HEIGHT_2};

    // Check if touched
    if(isTouched(&button1, &p)){return 1;}
    if(isTouched(&button2, &p)){return 2;}
    if(isTouched(&button3, &p)){return 3;}
    if(isTouched(&button4, &p)){return 4;}
    if(isTouched(&button5, &p)){return 5;}
    if(isTouched(&button6, &p)){return 6;}
    if(isTouched(&button7, &p)){return 7;}
    if(isTouched(&button8, &p)){return 8;}
    if(isTouched(&button9, &p)){return 9;}
    if(isTouched(&button10, &p)){return 10;}
    if(isTouched(&button11, &p)){return 11;}
    if(isTouched(&button12, &p)){return 12;}
    if(isTouched(&button13, &p)){return 13;}
    if(isTouched(&button14, &p)){return 14;}
 
    // Case of touched not on a button
    return 0;
  }
}

int touch_date1()
{
   // Retrieve a point  
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return 0;
  } else {
    // Scale from ~0->SPACE_NON_ADMIN00 to tft.width using the calibration #'s and rotates 
    rotateAndScaleTouch(&p);
    // Create the button for the small coffee choice 
    Button button1 = {262,8,BUTTON_WIDTH_3,BUTTON_HEIGHT_3};
    Button button2 = {40,24,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button3 = {110,24,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button4 = {180,24,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button5 = {40,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button6 = {110,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button7 = {180,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button8 = {262,182,BUTTON_WIDTH_3,BUTTON_HEIGHT_3};
    // Check if touched
    

    if(isTouched(&button1, &p)){return 1;}
    if(isTouched(&button2, &p)){return 2;}
    if(isTouched(&button3, &p)){return 3;}
    if(isTouched(&button4, &p)){return 4;}
    if(isTouched(&button5, &p)){return 5;}
    if(isTouched(&button6, &p)){return 6;}
    if(isTouched(&button7, &p)){return 7;}
    if(isTouched(&button8, &p)){return 8;}
    // Case of touched not on a button
    return 0;
  }
}
     
int touch_date2()
{
   // Retrieve a point  
  TSPoint p = ts.getPoint();
  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
     return 0;
  } else {
    // Scale from ~0->SPACE_NON_ADMIN00 to tft.width using the calibration #'s and rotates 
    rotateAndScaleTouch(&p);
    // Create the button for the small coffee choice 
    Button button1 = {262,8,BUTTON_WIDTH_3,BUTTON_HEIGHT_3};
    Button button2 = {40,24,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button3 = {110,24,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button4 = {180,24,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button5 = {40,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button6 = {110,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button7 = {180,160,BUTTON_WIDTH_4,BUTTON_HEIGHT_4};
    Button button8 = {262,182,BUTTON_WIDTH_3,BUTTON_HEIGHT_3};
    // Check if touched
    if(isTouched(&button1, &p)){return 1;}
    if(isTouched(&button2, &p)){return 2;}
    if(isTouched(&button3, &p)){return 3;}
    if(isTouched(&button4, &p)){return 4;}
    if(isTouched(&button5, &p)){return 5;}
    if(isTouched(&button6, &p)){return 6;}
    if(isTouched(&button7, &p)){return 7;}
    if(isTouched(&button8, &p)){return 8;}
    // Case of touched not on a button
    return 0;
  }
}


//**********************************************************************************************************
// FOR EACH STATE TFT **************************************************************************************
//**********************************************************************************************************

void tft_waitrfid()
{
  //jpegDraw("wait.jpg"); 
  jpegDraw("wait.jpg"); 
}

void tft_thankyou()
{
  jpegDraw("good.jpg"); 
}

void tft_nonadmin()
{
  jpegDraw("sele.jpg"); 
}

void tft_admin()
{
  jpegDraw("sela.jpg");
}

void tft_mycoffee()
{
  jpegDraw("good.jpg");
}
  
void tft_othercoffee()
{
   jpegDraw("good.jpg");
}
    
void tft_mycoffeesetup()
{
   jpegDraw("seco.jpg");
}
     
void tft_setup()
{
   jpegDraw("admi.jpg");
}
    
void tft_lock()
{
   jpegDraw("nofu.jpg");
}

void tft_date1()
{
   jpegDraw("dati.jpg");
}
    
void tft_date2()
{
   jpegDraw("dati.jpg");
}

void tft_notenough()
{
  tft.fillRect(0, 0, 320, 240, ILI9341_BLACK);
  tft_print("You have not enough money", 13 , 100, 2, ILI9341_WHITE, ILI9341_BLACK);
}

void tft_select_update(String minmoney,String usersmoney,String normcoffee,String expcoffee)
{
  float f_usersmoney = atof(usersmoney.c_str());
  String buff = "";
  buff = buff + usersmoney + "Euros   ";
  if(f_usersmoney < 0 )
  {
    buff = buff + expcoffee + "/coffee";
  }else{
    buff = buff + normcoffee + "/coffee";
  }
  tft_print(buff, 13 , 100, 2, ILI9341_WHITE, ILI9341_BLACK);
}

void tft_wait_update(int code_size)
{
  int i = 0;
  String buff = "";
  for(int i = 0; i<code_size; i++)
  {
     buff += "*"; 
  }
  tft.fillRect(50, 230, 270, 240, ILI9341_BLACK);
  tft_print(buff, 50 , 230, 2, ILI9341_WHITE, ILI9341_BLACK);
}

void tft_mycoffee_update(int beans, int water)
{
  tft.fillRect(98, 92, 70, 60, ILI9341_BLACK);
  tft_print(String(beans), 100 , 115, 2, ILI9341_WHITE, ILI9341_BLACK);
  
  tft.fillRect(98, 160, 70, 60, ILI9341_BLACK);
  tft_print(String(water), 100 , 183, 2, ILI9341_WHITE, ILI9341_BLACK);
}

void tft_datetime_update(int dat1, int dat2, int dat3)
{
  tft.fillRect(40, 92, 60, 60, ILI9341_BLACK);
  tft_print(String(dat1), 42 , 115, 2, ILI9341_WHITE, ILI9341_BLACK);
  
  tft.fillRect(110, 92, 60, 60, ILI9341_BLACK);
  tft_print(String(dat2), 130 , 115, 2, ILI9341_WHITE, ILI9341_BLACK);
  
  tft.fillRect(180, 92, 60, 60, ILI9341_BLACK);
  tft_print(String(dat3), 200 , 115, 2, ILI9341_WHITE, ILI9341_BLACK);
}
  
//**********************************************************************************************************
// FOR EACH STATE SD DRAW BMP ******************************************************************************
//**********************************************************************************************************
void bmpDraw(char *filename, uint8_t x, uint16_t y) {

  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

   if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  
  if (bmpFile.open(filename,O_RDONLY) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print(F("File size: ")); Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print(F("Image Offset: ")); Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print(F("Header size: ")); Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print(F("Bit Depth: ")); Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);
        //Serial.print(x+w-1);Serial.print("-");Serial.print(y+h-1);
        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }
            
            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print(F("Loaded in "));
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println(F("BMP format not recognized."));
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

//**********************************************************************************************************
// FOR EACH STATE SD DRAW jpeg ******************************************************************************
//**********************************************************************************************************
unsigned char pjpeg_need_bytes_callback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data)
{
   uint n;
   pCallback_data;
   
   n = min(g_nInFileSize - g_nInFileOfs, buf_size);
   if (n && (g_pInFile.read(pBuf, n) != n))
      return PJPG_STREAM_READ_ERROR;
   *pBytes_actually_read = (unsigned char)(n);
   g_nInFileOfs += n;
   return 0;
}

void jpegDraw(char *pFilename)
{
 uint8_t r;
 uint8_t g; 
 uint8_t b;
 int mcu_x = 0;
 int mcu_y = 0;
 int reduce = 0;

 pjpeg_scan_type_t *pScan_type;
 pjpeg_image_info_t image_info;

 unsigned int row_pitch;
 uint8 status;
 uint decoded_width, decoded_height;
 uint row_blocks_per_mcu, col_blocks_per_mcu;

 if (pScan_type) *pScan_type = PJPG_GRAYSCALE;

 if (g_pInFile.open(pFilename,O_RDONLY) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

 g_nInFileOfs = 0; 
 g_nInFileSize = g_pInFile.size(); 
    
 status = pjpeg_decode_init(&image_info, pjpeg_need_bytes_callback, NULL);    
 if (status)
 {
    Serial.print("pjpeg_decode_init() failed with status");
    if (status == PJPG_UNSUPPORTED_MODE)
    {
       Serial.print("Progressive JPEG files are not supported.\n");
    }

    g_pInFile.close();
    return ;
 }
 
 if (pScan_type){ *pScan_type = image_info.m_scanType; }

 // In reduce mode output 1 pixel per 8x8 block.
 decoded_width = reduce ? (image_info.m_MCUSPerRow * image_info.m_MCUWidth) / 8 : image_info.m_width;
 decoded_height = reduce ? (image_info.m_MCUSPerCol * image_info.m_MCUHeight) / 8 : image_info.m_height;

 row_blocks_per_mcu = image_info.m_MCUWidth >> 3;
 col_blocks_per_mcu = image_info.m_MCUHeight >> 3;
 
 int tftx = 0;
  
 for ( ; ; )
 {
    int y, x;
    uint8 *pDst_row;

    status = pjpeg_decode_mcu((unsigned char)reduce);
    
    if (status)
    {
       if (status != PJPG_NO_MORE_BLOCKS)
       {
          Serial.print("pjpeg_decode_mcu() failed with status \n");
          g_pInFile.close();
          return;
       }

       break;
    }

    if (mcu_y >= image_info.m_MCUSPerCol)
    {
       Serial.print("mcu_y >= image_info.m_MCUSPerCol");
       g_pInFile.close();
       return ;
    }

   for (y = 0; y < image_info.m_MCUHeight; y += 8)
   {
      const int by_limit = min(8, image_info.m_height - (mcu_y * image_info.m_MCUHeight + y));
      for (x = 0; x < image_info.m_MCUWidth; x += 8)
      {
         // Compute source byte offset of the block in the decoder's MCU buffer.
         uint src_ofs = (x * 8U) + (y * 16U);
         const uint8 *pSrcR = image_info.m_pMCUBufR + src_ofs;
         const uint8 *pSrcG = image_info.m_pMCUBufG + src_ofs;
         const uint8 *pSrcB = image_info.m_pMCUBufB + src_ofs;

         const int bx_limit = min(8, image_info.m_width - (mcu_x * image_info.m_MCUWidth + x));

          int bx, by;
          int posy = (int)(tftx/40)*8;
          int posx = (tftx - posy*5 ) * 8;
          tft.setAddrWindow(posx,posy,posx + 7,posy + 7);
          //Serial.print(posx);Serial.print("-");Serial.print(posy);Serial.print("-");Serial.println(tftx);
          tftx++;
          
          for (by = 0; by < by_limit; by++)
          {
             for (bx = 0; bx < bx_limit; bx++)
             {
                r = *pSrcR++; 
                g = *pSrcG++; 
                b = *pSrcB++; 
                tft.pushColor(tft.color565(r,g,b));
                //Serial.print(r);Serial.print("-");Serial.print(g);Serial.print("-");Serial.println(b);
                
             }
             pSrcR += (8 - bx_limit);
             pSrcG += (8 - bx_limit);
             pSrcB += (8 - bx_limit);
          }
      }
    }
    mcu_x++;
    if (mcu_x == image_info.m_MCUSPerRow)
    {
       mcu_x = 0;
       mcu_y++;
    }
 }

 g_pInFile.close();
}


