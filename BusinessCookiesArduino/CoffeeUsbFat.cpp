#include "CoffeeUsbFat.h"

// Setup 

boolean setup_RTCM()
{
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(F("--------------------"));
  Serial.println(F("Time Module Setup"));
  Serial.println(F("--------------------"));
  #endif
  RTC.read(tm);
  #ifdef COFFEEUSBFATDEBUG
  Serial.print("Ok, Time = ");
  Serial.print(tm.Hour);
  Serial.write(':');
  Serial.print(tm.Minute);
  Serial.write(':');
  Serial.print(tm.Second);
  Serial.print(", Date (D/M/Y) = ");
  Serial.print(tm.Day);
  Serial.write('/');
  Serial.print(tm.Month);
  Serial.write('/');
  Serial.print(tmYearToCalendar(tm.Year));
  Serial.println();
  #endif
  if (RTC.chipPresent()) {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println("The DS1307 is Ok");
    #endif
    return true;
    
  } else {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println("DS1307 read error!  Please check the circuitry.");
    #endif
    return false;
    
  }
}


boolean setup_serial()
{
  Serial2.begin(9600);
  serial2Flush();
  return true;
}

boolean setup_usbdrive()
{
  #ifdef COFFEEUSBFATDEBUG
  Serial.println();
  Serial.println(F("--------------------"));
  Serial.println(F("UsbFat Setup"));
  Serial.println(F("--------------------"));
  #endif
  
  // Initialize the USB bus.
  if (!initUSB(&usb)) {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("initUSB failed"));
    #endif
    return false;    
  }
  // Init the USB key or USB hard drive.
  if (!key.begin()) {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("key.begin failed"));
    #endif
    return false;
  }
  
  return true;
}

void serial2Flush()
{
  while(Serial2.available() > 0)
  {
    char t = Serial2.read();
  }
}   

boolean update_DB()
{
  serial2Flush();
  Serial2.write(BYTE_STA);
  delay(2000);
  if(Serial2.read() != BYTE_ACK)
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(Serial2.available());
    Serial.println(F("No response ciao bye bye:"));
    #endif
    return false;
  }

  // Send then delete date.csv
  while(!file_date.open("date.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open date.csv"));
    #endif
  }  
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(F("Sending date.csv:"));
  #endif
  char r = -1;
  file_date.seek(0);
  while(file_date.available() != 0)
  {
      r = file_date.read();
      Serial2.write(r);
      #ifdef COFFEEUSBFATDEBUG
      Serial.print(r);
      #endif
      while(Serial2.read() != BYTE_ACK){}
  }
  Serial2.write(BYTE_END);
  while(Serial2.read() != BYTE_ACK){}  
  file_date.close();
  file_date.open("date.csv", O_WRITE | O_TRUNC);
  file_date.close();
  
  // Delete and receive new admin file
  file_admin.open("admin.csv", O_WRITE | O_TRUNC);
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(F("Receiving admin.csv:"));
  #endif
  while(Serial2.available() == 0){}  
  r = Serial2.read(); 
  Serial2.write(BYTE_ACK);
  while(r != BYTE_END)
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(r);
    #endif
    file_admin.write(r);
    while(Serial2.available() == 0){}
    r = Serial2.read();
    Serial2.write(BYTE_ACK);
  }
  Serial2.write(BYTE_ACK);
  file_admin.close();
  
  // Delete and receive new minmoney file
  file_minmoney.open("minmoney.csv", O_WRITE | O_TRUNC);
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(F("Receiving minmoney.csv:"));
  #endif
  // Wait for raspi to process information
  while(Serial2.available() == 0){}  
  r = Serial2.read();
  while(r != BYTE_END)
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(r);
    #endif
    file_minmoney.write(r);
    while(Serial2.available() == 0){}
    r = Serial2.read();
    Serial2.write(BYTE_ACK);
  }
  Serial2.write(BYTE_ACK);
  file_minmoney.close();

 // Delete and receive new minmoney file
  file_normcoffee.open("normcoffee.csv", O_WRITE | O_TRUNC);
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(F("Receiving normcoffee.csv:"));
  #endif
  // Wait for raspi to process information
  while(Serial2.available() == 0){}  
  r = Serial2.read();
  while(r != BYTE_END)
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(r);
    #endif
    file_normcoffee.write(r);
    while(Serial2.available() == 0){}
    r = Serial2.read();
    Serial2.write(BYTE_ACK);
  }
  Serial2.write(BYTE_ACK);
  file_normcoffee.close();

 // Delete and receive new minmoney file
  file_expcoffee.open("expcoffee.csv", O_WRITE | O_TRUNC);
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(F("Receiving expcoffee.csv:"));
  #endif
  // Wait for raspi to process information
  while(Serial2.available() == 0){}  
  r = Serial2.read();
  while(r != BYTE_END)
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(r);
    #endif
    file_expcoffee.write(r);
    while(Serial2.available() == 0){}
    r = Serial2.read();
    Serial2.write(BYTE_ACK);
  }
  Serial2.write(BYTE_ACK);
  file_expcoffee.close();
  
  return true;
}

String csv_itoa10(int toConvert, int leng)
{
  char converted[DATE_STRING_BUFFER_SIZE];
  itoa(toConvert,converted,10);
  int diff = leng - String(converted).length();
  String returned = ""; 
  for(int i = 0; i < diff; i++)
  {
    returned += "0";
  } 
  returned += converted;
  return returned; 
}

String csv_getMinMoney()
{
  //opening files 
  while(!file_minmoney.open("minmoney.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open minmoney.csv"));
    #endif
  }
  char r = -1;
  file_minmoney.seek(0);
  char value[BUFFER_SIZE];
  r = file_minmoney.read();
  int number = 0;
  while(file_minmoney.available() != 0)
  {
      value[number] = r;
      number++;
      r = file_minmoney.read();  
  }
  value[number] = '\0';
  String str_value = String(value);
  file_minmoney.close();
  return str_value;
}

String csv_getNormCoffee()
{
  //opening files 
  while(!file_normcoffee.open("normcoffee.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open minmoney.csv"));
    #endif
  }
  char r = -1;
  file_normcoffee.seek(0);
  char value[BUFFER_SIZE];
  r = file_normcoffee.read();
  int number = 0;
  while(file_normcoffee.available() != 0)
  {
      value[number] = r;
      number++;
      r = file_normcoffee.read();  
  }
  value[number] = '\0';
  String str_value = String(value);
  file_normcoffee.close();
  return str_value;
}

String csv_getExpCoffee()
{
  //opening files 
  while(!file_expcoffee.open("expcoffee.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open minmoney.csv"));
    #endif
  }
  char r = -1;
  file_expcoffee.seek(0);
  char value[BUFFER_SIZE];
  r = file_expcoffee.read();
  int number = 0;
  while(file_expcoffee.available() != 0)
  {
      value[number] = r;
      number++;
      r = file_expcoffee.read();  
  }
  value[number] = '\0';
  String str_value = String(value);
  file_expcoffee.close();
  return str_value;
}

// write a choice in the file_date
void csv_writeChoice(String rfid,int choice)
{
  /*RTC.read(tm);
  String date = "";
  date += csv_itoa10(tmYearToCalendar(tm.Year), 4);
  date += csv_itoa10(tm.Month, 2);
  date += csv_itoa10(tm.Day, 2);
  date += csv_itoa10(tm.Hour, 2);
  date += csv_itoa10(tm.Minute, 2);
  date += csv_itoa10(tm.Second, 2);
  
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(date);
  #endif
  */
  while(!file_date.open("date.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open date.csv"));
    #endif
  }
  
  file_date.seek(file_date.size()-2);
  if(file_date.read() != 13 || file_date.read() != 10)
  {
    file_date.println();
  }
  String date = "19700101000001";
  #ifdef COFFEEUSBFATDEBUG
  Serial.println(date);
  Serial.println(rfid);
  Serial.println(choice);
  #endif
    
  file_date.print(date);
  file_date.print(';');
  file_date.print(rfid);
  file_date.print(';');
  file_date.println(choice);
  
  delay(200);
  file_date.close();
  delay(200);  
}

String csv_getUsersMoney(String rfid)
{
  //opening files 
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Check if admin: "));
  Serial.println(rfid);
  #endif
  
  int pos = csv_findValue(file_admin, 1, rfid );
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Position: "));
  Serial.println(pos);
  #endif
  String value = "-1000";
  if(pos == -1)
  {
    file_admin.close();
    return value;
  }else{
    value = csv_getValue(file_admin, 3,pos);
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(F("Linked value: "));
    Serial.println(value);
    #endif
  }
  file_admin.close();
  return value; // see if debug more than that 
}

String csv_retrieveRfid(String code)
{
  //opening files 
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Retrieve rfid for code: "));
  Serial.println(code);
  #endif
  
  int pos = csv_findValue(file_admin, 4, code);
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Position: "));
  Serial.println(pos);
  #endif
  if(pos == -1)
  {
    file_admin.close();
    return code;
  }else{
    String value = csv_getValue(file_admin, 1,pos);
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(F("Linked value: "));
    Serial.println(value);
    #endif
    file_admin.close();
    return value;
  } 
}

int csv_isCodeAdmin(String code)
{
  //opening files 
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Check if admin: "));
  Serial.println(code);
  #endif
  
  int pos = csv_findValue(file_admin, 4, code );
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Position: "));
  Serial.println(pos);
  #endif
  if(pos == -1)
  {
    file_admin.close();
    return -1;
  }else{
    String value = csv_getValue(file_admin, 2,pos);
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(F("Linked value: "));
    Serial.println(value);
    #endif
    if(value == "ja")
    {
      file_admin.close();
      return STATE_ADMIN;    
    }else if(value == "nein"){
      file_admin.close();
      return STATE_NONADMIN;
    }
  }
  file_admin.close();
  return -1; // see if debug more than that 
}

// look for the given rfid (string) in the file_log
// if found and admin, returns 1
// if found and not admin, return 0
// if not found, returns -1

int csv_isIdAdmin(String rfid)
{
  //opening files 
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Check if admin: "));
  Serial.println(rfid);
  #endif
  
  int pos = csv_findValue(file_admin, 1, rfid );
  #ifdef COFFEEUSBFATDEBUG
  Serial.print(F("Position: "));
  Serial.println(pos);
  #endif
  if(pos == -1)
  {
    file_admin.close();
    return -1;
  }else{
    String value = csv_getValue(file_admin, 2,pos);
    #ifdef COFFEEUSBFATDEBUG
    Serial.print(F("Linked value: "));
    Serial.println(value);
    #endif
    if(value == "ja")
    {
      file_admin.close();
      return STATE_ADMIN;    
    }else if(value == "nein"){
      file_admin.close();
      return STATE_NONADMIN;
    }
  }
  file_admin.close();
  return -1; // see if debug more than that 
}

// get the value on the given column on the same line as postition
// pos must be the begining of the line 
String csv_getValue(File file, int number_column, int pos )
{
  file.seek(pos);
  if(pos == -1) return "";
  int it_col = 1; 
  char r = -1;
  while(it_col != number_column)
  {
     r = file.read();
     if(r == ';')
     {
       it_col++;
     }   
  } 
  int number = 0;
  char value[BUFFER_SIZE];
  r = file.read();
  while(r != ';' && r != 13 && r != 10 && r != -1)
  {
      value[number] = r;
      number++;
      r = file.read();  
  }
  value[number] = '\0';
  String str_value = String(value);
  return str_value;
}

// try to find the given value in a given column (1 to ...)
// if found, it return the position of the found value (just before the first letter of the line)
// if not, it returns -1
int csv_findValue(File file, int number_column, String value )
{
  file.seek(0);
  int pos = 0; 
  char r = -1;
  int it_col = 0;
  while(file.available() != 0)
  {
    it_col++;
    r = file.read();
    if(r == ';')it_col++;  
    while(it_col != number_column)
    {
       r = file.read();
       if(r == ';')
       {
         it_col++;
         r = file.read();
       }
       if(r == 13 || r == 10 || r == -1)
       {
          it_col = 1;
          r = file.read();
          while(r == 13 || r == 10)r = file.read();
          pos = file.position()-1;
       }
    } 
    int number = 0;
    char comp_value[BUFFER_SIZE];
    while(r != ';' && r != 13 && r != 10 && r != -1)
    {
        comp_value[number] = r;
        number++;
        r = file.read();  
    }
    comp_value[number] = '\0';
    String str_value = String(comp_value);
    if(value == str_value)
    {
       return pos;
    }     
  }
  file.seek(0);
  return -1;
}

// get the number of column of a csv file, given the first line
// the CRLF (end of line) condition is true if r = LF
int csv_getNumberColumn(File file)
{
  file.seek(0);
  char r = file.read();
  int number = 1;
  while(r != 10 && r != 13)
  {
    if(r == ';')number++;
    r = file.read();  
  }
  file.seek(0);
  return number;  
}

// get the number of line of a csv file
// the CRLF (end of line) condition is true if r = LF
int csv_getNumberLine(File file)
{
  file.seek(0);
  char r = file.read();
  int number = 1;
  while(file.available() != 0)
  {
    if(r == 10)number++;
    r = file.read(); 
  }
  file.seek(0);
  return number;  
}

int get_csv_beans(String rfid)
{
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  int pos = csv_findValue(file_admin, 1, rfid );
  String value = csv_getValue(file_admin, 3,pos);
  file_admin.close();
  return value.toInt();
}

int get_csv_water(String rfid)
{
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  int pos = csv_findValue(file_admin, 1, rfid );
  String value = csv_getValue(file_admin, 4,pos);
  file_admin.close();
  return value.toInt();
}

void set_csv_beans(String rfid, int beans)
{
  String value = csv_itoa10(beans, 1);
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  int pos = csv_findValue(file_admin, 1, rfid );
  file_admin.seek(pos); 
  char r = '0';
  int it_col = 1;
  int number_column = 3;
  while(it_col != number_column)
  {
     r = file_admin.read();
     if(r == ';')
     {
       it_col++;
     }     
  }
  file_admin.print(value);
  file_admin.close();
}

void set_csv_water(String rfid,int water)
{
  String value = csv_itoa10(water, 3);
  while(!file_admin.open("admin.csv", O_CREAT | O_RDWR))
  {
    #ifdef COFFEEUSBFATDEBUG
    Serial.println(F("can't open admin.csv"));
    #endif
  }
  int pos = csv_findValue(file_admin, 1, rfid );
  file_admin.seek(pos);
  char r = '0';
  int it_col = 1;
  int number_column = 4;
  while(it_col != number_column)
  {
     r = file_admin.read();
     if(r == ';')
     {
       it_col++;
     }     
  }
  file_admin.print(value);
  file_admin.close();
}

void set_int_date(int datY, int datM, int datD)
{
  /*tm.Day = datD;
  tm.Month = datM;
  tm.Year = CalendarYrToTm(datY);
  RTC.write(tm);*/
}

void set_int_time(int dath, int datm, int dats)
{
  /*tm.Hour = dath;
  tm.Minute = datm;
  tm.Second = dats;
  RTC.write(tm);*/
}

int get_int_datetime(char type)
{
  //RTC.read(tm);
  switch(type)
  {
     case 'Y':
       //return tmYearToCalendar(tm.Year);
       return 1970;
       break; 
     case 'M':
       //return tm.Month;
       return 1;
       break;
     case 'D':
       //return tm.Day;
       return 1;
       break;
     case 'h':
       //return tm.Hour;
       return 0;
       break;
     case 'm':
       //return tm.Minute;
       return 0;
       break;
     case 's':
      //return tm.Second;
       return 1;
       break;
  }
}
