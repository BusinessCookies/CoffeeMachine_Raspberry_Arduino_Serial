/*
This file is just used to make the main file "project.ino" clearer. Here you can find all the fonctions dealing
with the different states represented by the switch statement in the main loop function.
*/

#ifndef STATES_H
#define STATES_H
int previous_state = STATE_WAITRFID;
int current_state = STATE_WAITRFID;
int choice = 0;

unsigned long time_init;
unsigned long time_now; 
String rfid = RFID_NULL;
String code = CODE_NULL;

/* DONE
Wait for an rfid to be identified.
csv_isIdAdmin return the state corresponding of the ID:
ADMIN, NONADMIN or WAITRFID if unknown
*/
void state_waitrfid()
{
  rfid = RFID_NULL;
  code = CODE_NULL;
  tft_waitrfid();
  int val = touch_waitrfid();
  while(rfid == RFID_NULL && code.length() < 5 )
  {
    if(val == 11)
    {
       pressSimul(PIN_BUTTON_ST);
    }else if(val == 12){
       pressSimul(PIN_BUTTON_CL);
    }else if(val == 13){
       pressSimul(PIN_BUTTON_RP); 
    }else if(val <= 9 && val >= 0){
       char buff[1];
       itoa(val,buff, 10);
       code = code + buff; 
    }else if(val == 10){
       code = code.substring(0,code.length()-1); 
    }
    tft_wait_update(code.length());
    rfid = rfid_getId();
    if(val != -1)delay(300);
    val = touch_waitrfid();
  }
  
  if(rfid != RFID_NULL)
  {  
    current_state = csv_isIdAdmin(rfid);
  }
  
  if(code.length() == 5)
  {
    current_state = csv_isCodeAdmin(code);
    if(current_state != -1)
    {
      rfid = csv_retrieveRfid(code);
    }
  } 
}
 
/* 
Return the next state depending on the choice
1,2,4,5: OTHERCOFFEE
3   : MYCOFFEE
6   : MYCOFFEESETUP
*/
void state_nonadmin()
{
  String minmoney = csv_getMinMoney();
  String usersmoney = csv_getUsersMoney(rfid);
  String normcoffee = csv_getNormCoffee();
  String expcoffee = csv_getExpCoffee();
  float f_minmoney = atof(minmoney.c_str());
  float f_usersmoney = atof(usersmoney.c_str());
  if(f_usersmoney < f_minmoney )
  {
    current_state = STATE_NOTENOUGH;
    return;
  }
  tft_nonadmin();
  choice = touch_nonadmin();
  time_init = millis();
  time_now = millis();
  while(choice == 0 && (time_now - time_init) < CHOICE_TIMEOUT)
  {
    choice = touch_nonadmin();
    tft_select_update(minmoney, usersmoney, normcoffee, expcoffee);
    time_now = millis();
  }
  if( (time_now - time_init) >= CHOICE_TIMEOUT)
  {
    current_state = STATE_WAITRFID;
  }else if(choice == 1 || choice == 2 || choice == 4 || choice == 5 ){
    csv_writeChoice(rfid, choice);
    current_state = STATE_OTHERCOFFEE;
  }else if(choice == 3 ){
    current_state = STATE_MYCOFFEESETUP;
  }else if(choice == 6 ){
    csv_writeChoice(rfid, choice);
    current_state = STATE_MYCOFFEE;
  } 
}

void state_notenough()
{
   tft_notenough();
   delay(2000);
   current_state = STATE_WAITRFID;
}
  
/* DONE
Return the next state depending on the choice
1, 4, 5: OTHERCOFFEE
2      : SETUP 
3      : MYCOFFEE
6      : MYCOFFEESETUP
*/
void state_admin()
{
  tft_admin();
  choice = touch_admin();
  String minmoney = csv_getMinMoney();
  String usersmoney = csv_getUsersMoney(rfid);
  String normcoffee = csv_getNormCoffee();
  String expcoffee = csv_getExpCoffee();   
  time_init = millis();
  time_now = millis();
  while(choice == 0 && (time_now - time_init) < CHOICE_TIMEOUT)
  {
    choice = touch_admin();
    tft_select_update(minmoney, usersmoney, normcoffee, expcoffee);
    time_now = millis();
  }
  if( (time_now - time_init) >= CHOICE_TIMEOUT)
  {
    current_state = STATE_WAITRFID;
  }else if(choice == 1 || choice == 4 || choice == 5){
    csv_writeChoice(rfid, choice);
    current_state = STATE_OTHERCOFFEE;
  }else if(choice == 2 ){
    current_state = STATE_SETUP;
  }else if(choice == 3 ){
    current_state = STATE_MYCOFFEESETUP;
  }else if(choice == 6 ){
    current_state = STATE_MYCOFFEE;
  }
}
 
/* DONE
Diplay "thank you" and reroute to WAITRFID
*/ 
void state_thankyou()
{
  tft_thankyou();
  update_DB();
  current_state = STATE_WAITRFID; 
}

/* 
  TO IMPROVE
*/
void state_mycoffee()
{
  pressSimul(PIN_BUTTON_SB);
  tft_thankyou();
  // For bean
  delay(TIME_DELAY_MYCOFFEE1);
  int bean_count = AMOUNT_BEAN_SB - get_csv_beans(rfid);
  Serial.println(bean_count);
  if(bean_count > 0){
    for(int i=0; i < (int)(bean_count); i++ )
    {
        rotaLeftSimul();
        delay(TIME_ROTA_DELAY);
    }
  }else{
    for(int i=0; i > (int)(bean_count); i-- )
    {
        rotaRightSimul();
        delay(TIME_ROTA_DELAY);
    }
  }
  
  // For water
  delay(TIME_DELAY_MYCOFFEE2);
  int water_count = (int)( (AMOUNT_WATER_SB - get_csv_beans(rfid))/5 );
  Serial.println(water_count);
  if(water_count > 0){
    for(int i=0; i < water_count; i++ )
    {
        rotaLeftSimul();
        delay(TIME_ROTA_DELAY);
    }
  }else{
    for(int i=0; i > water_count; i-- )
    {
        rotaRightSimul();
        delay(TIME_ROTA_DELAY);
    }
  }
  current_state = STATE_THANKYOU;
}
 
/* DONE
Simul the button for the choosen button 
*/
void state_othercoffee()
{
  int pin_button = 0;
  switch(choice)
  {
     case 1:
       pin_button = PIN_BUTTON_SS;
       break; 
     case 2:
       pin_button = PIN_BUTTON_DS;
       break;
     case 4:
       pin_button = PIN_BUTTON_SB;
       break;
     case 5:
       pin_button = PIN_BUTTON_DB;
       break;
  }
   pressSimul(pin_button);
   current_state = STATE_THANKYOU;
}
 
/*
DONE
*/
void state_mycoffeesetup()
{
   tft_mycoffeesetup();
   choice = touch_mycoffeesetup();
  
   int beans = get_csv_beans(rfid);
   int water = get_csv_water(rfid);
   
   tft_mycoffee_update(beans, water);
   
   while(choice != 1)
   {
     choice = touch_mycoffeesetup(); 
     switch(choice)
     {
     case 2:
       if(beans > 1){beans -= 1;}
       break; 
     case 3:
       if(beans < 5){beans += 1;}
       break; 
     case 4:
       if(water > 50){water -= 5;}
       break; 
     case 5:
       if(water < 250){ water += 5; }
       break; 
     }
     
     if(choice != 0)
     {
        tft_mycoffee_update(beans, water);
     }
   }
   
   if(choice == 1)
   {
     set_csv_beans(rfid,beans);
     set_csv_water(rfid,water);
     current_state = csv_isIdAdmin(rfid);
   }
}


/* DONE 
To simulate all the button, and go to time setup
*/
void state_setup()
{
  if (previous_state != STATE_SETUP)
  {
      tft_setup();
  }
  choice = touch_setup();
  
  while(choice == 0)
  {
    choice = touch_setup();
  }
  
  switch(choice)
  {
     case 1:
       pressSimul(PIN_BUTTON_ST);
       current_state = STATE_SETUP;
       break; 
     case 2:
       pressSimul(PIN_BUTTON_CL);
       current_state = STATE_SETUP;
       break;
     case 3:
       current_state = STATE_LOCK;
       break;
     case 4:
       pressSimul(PIN_BUTTON_SS);
       current_state = STATE_SETUP;
       break;
       case 5:
       pressSimul(PIN_BUTTON_DS);
       current_state = STATE_SETUP;
       break; 
     case 6:
       pressSimul(PIN_BUTTON_RP);
       current_state = STATE_SETUP;
       break;
     case 7:
       pressSimul(PIN_BUTTON_SB);
       current_state = STATE_SETUP;
       break;
     case 8:
       pressSimul(PIN_BUTTON_DB);
       current_state = STATE_SETUP;
       break;
       case 9:
       pressSimul(PIN_BUTTON_CC);
       current_state = STATE_SETUP;
       break; 
     case 10:
       rotaLeftSimul();
       current_state = STATE_SETUP;
       break;
     case 11:
       longPressSimulRP();
       current_state = STATE_SETUP;
       break;
     case 12:
       rotaRightSimul();
       current_state = STATE_SETUP;
       break;
     case 13:
       current_state = STATE_DATE1;
       break;
     case 14:
       current_state = STATE_ADMIN;
       break;
  }
  previous_state = current_state;
}
   
/* DONE 
Lock the screen
*/
void state_lock()
{
  tft_lock();
  rfid = RFID_NULL;
  
  while(rfid == RFID_NULL)
  {
    rfid = rfid_getId();
  }
  
  if(csv_isIdAdmin(rfid) == true)
  {
    current_state = STATE_ADMIN;
  }else{
    current_state = STATE_LOCK;
  }
  
}

/* DONE
To change the day, month and year
*/
void state_date1()
{
   tft_date1();
   choice = touch_date1();
   int datY = get_int_datetime('Y');
   int datM = get_int_datetime('M');
   int datD = get_int_datetime('D');
   tft_datetime_update(datY, datM, datD);
   
   while(choice != 1 && choice != 8)
   {
     choice = touch_date1(); 
     switch(choice)
     {
     case 2:
       datY++;
       break; 
     case 3:
       datM++;
       break; 
     case 4:
       datD++;
       break; 
     case 5:
       datY--;
       break; 
     case 6:
       datM--;
       break; 
     case 7:
       datD--;
       break; 
     }
     
     if(choice != 0)
     {
       tft_datetime_update(datY, datM, datD);
     }
   }
   
   
   if(choice == 1)
   {
     current_state = STATE_SETUP;
   }
   
   if(choice == 8)
   {
     set_int_date(datY, datM, datD);
     current_state = STATE_DATE2;
   }
   
}

/* DONE
To change the hour, minute and second
*/     
void state_date2()
{
   tft_date2();
   choice = touch_date2();
   int dath = get_int_datetime('h');
   int datm = get_int_datetime('m');
   int dats = get_int_datetime('s');
   tft_datetime_update(dath, datm, dats);
   
   while(choice != 4 && choice != 8)
   {
     choice = touch_date2(); 
     switch(choice)
     {
     case 2:
       dath++;
       break; 
     case 3:
       datm++;
       break; 
     case 4:
       dats++;
       break; 
     case 5:
       dath--;
       break; 
     case 6:
       datm--;
       break; 
     case 7:
       dats--;
       break; 
     }
     if(choice != 0)
     {
       tft_datetime_update(dath, datm, dats);
     }
   }
   
   if(choice == 4)
   {
     current_state = STATE_DATE1;
   }else if(choice == 8){
     set_int_time(dath, datm, dats);
     current_state = STATE_SETUP;
   }
   
}

#endif
