/*************************************************************************************************************************************************
  HARDWARCALIBRATIONE             = NITHIN & RAJATH
  SOFTWARE             = NITHIN N
  SOFTWARE VERSION     = 8014 V7
  SOFTWARE REQUIRMENT  = -----------------------
                     1) AUTOMATIC SELECTION C&F UNIT
                     2) ALRAM SETTINGS
                     3) BUZER SOUND SELECTABLE
                     4)  FACTOR
                     5) PASSWORD CHECKING
                     6) SENSOE ERROR DETECTION
                     7) DISTANCE CALIBRATION
                     8) AMBINT COMPENSATION ALGORITHM

  MODIFICATION DATE  = 06-08-2020



**************************************************************************************************************************************************/
#include <Wire.h>

#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define   OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);
#define D6T_addr 0x0A
#define D6T_cmd 0x4C
#define  echoPin   11
#define  trigPin   12
#define  greenled  9
#define  redled    10
#define  buzer     2
#define  relay_1   2
#define  relay_2   2
#define  temp_limit 1000

// EEPROM adress
const byte adres_0   = 0;
const byte adres_1   = 2;
const byte adres_2   = 3;
const byte adres_3   = 4;
const byte adres_5   = 5;
const byte adres_6   = 6;
const byte adres_7   = 7;
const byte addr15   = 8;
const byte addr16   = 9;
const byte addr17   = 10;

byte unit_flg = 0,buzr_flg=0,alrm_flg=0;
byte alrm_tempF = 0;

// button pin
const byte select   = 5;
const byte enterpin = 8;
const byte dowenpin = 6;
const byte uppin    = 7;

byte  enter = 0;
byte  upp   = 0;
byte  dowen = 0;
byte  selec = 0;

// thermopile variable
int rbuf[35];
float tdata[16];
float t_PTAT;

// ultrasonic sensor
long duration;
int distance;
int set_val, low, high, password;
byte unit;
// function
void button_cal(void);
byte Dis_mesrmt(void);
float init_D6t();
byte Timer;

// calif
 float cali_f=0;
void EEPROMread(void);
void pass_check(void);
void setting_fn(void);
void beepon_off(void);
void password_check(void );
void password_set(void);
void password_set_decisoion(void );
  byte sensor_flag, Distance1,alrm_tempC=0;
  float t,cali_c=0;
  byte pass_word_status = 0;
 
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  pinMode(select,   INPUT);
  pinMode(uppin,    INPUT);
  pinMode(dowenpin, INPUT);
  pinMode(enterpin, INPUT);
  pinMode(trigPin,  OUTPUT);// Set the trigPin as an OUTPUT
  pinMode(echoPin,  INPUT);
  pinMode(greenled, OUTPUT);
  pinMode(redled,   OUTPUT);
  pinMode(buzer,    OUTPUT);

  display.begin();
  display.begin();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(42, 0);
  display.print("8013-IR");
  display.setCursor(20, 10);
  display.setTextSize(2);
  display.print("HEATCON");
  display.display();
  delay(3000);

}

void loop()
{
 
  EEPROMread();
  display.clearDisplay();
  
             
         button_cal();
         if (!selec)
          {
            delay(500);
            while(1)
              {
                  button_cal();
                  setting_fn();
                  if (!enter)
                   break;
              }
          }

    }
    



void button_cal(void)
{
  enter = digitalRead(enterpin);
  upp   = digitalRead(uppin);
  selec = digitalRead(select);
  dowen = digitalRead(dowenpin);
}
void beepon_off()
{
    if (buzr_flg == 0)
    digitalWrite(buzer, HIGH);
    else
    digitalWrite(buzer, LOW);
}

void EEPROMread(void)
{

   unit_flg  = EEPROM.read(adres_0);
   buzr_flg=   EEPROM.read(adres_1);
   alrm_tempF= EEPROM.read(adres_2);
   EEPROM.get(adres_3, cali_f);
}


void setting_fn(void)
{
  byte timeflag = 0, count = 0, count1 = 2,count3=1;
  delay(500);
  while (1)
  {
   
   // else{
    button_cal();
    if (!upp)
    {
      delay(250);
      count1++;
      if (count1 > 4)
      {
        count1 = 0;
      }
      if (count1 < 0)
        count1 = 0;

    }
    if (!dowen)
    {
      delay(250);
      count1--;
      if (count1 < 0)
        count1 = 0;
      if (count1 > 4)
      {
        count1 = 0;
      }

    }
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(10, 0);
      display.print("Settings....");
      display.setTextSize(2);
      display.setCursor(20, 15);
    if (count1 == 0)
    {
      display.print(" UNIT ");
      display.display();
    }
    else if (count1 == 1)
    {
     
      display.print("ALARM");
      display.display();
    }
    else if (count1 == 2)
    {
     
      display.print("BAZER");
      display.display();
    }
    else if (count1 == 3)
    {
     
      display.print("PASSWD");
      display.display();
      delay(1000);
    }
    else
    {
      display.print("EXIT");//
      display.display();
       if ((!enter) && (count1 == 3))
        {
         count1 = 0;
         delay(500);
         break;
        }
    }
    switch (count1)
    {
      case 0:
        {
          if ((!selec))
          {
            delay(250);
            count++;
            if (count > 1)
            {
              count = 0;
            }
            if (count == 0)
            {
              unit_flg =0;
              EEPROM.write(adres_0, unit_flg);
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(7, 10);
              display.print(" CELSIUS");
              display.display();
              delay(500);
            }
            else
            {
              unit_flg = 1;
              EEPROM.write(adres_0, unit_flg);
              display.clearDisplay();
              display.setTextSize(2);
              display.setCursor(7, 10);
              display.print("FAHRENHEIT");
              display.display(); delay(500);
            }

          }
        }
        break;
      case 1:
         {
               if(!selec)
                  {
                    while (1)
                       {
                          button_cal();
                          if (alrm_tempF < 95)
                          alrm_tempF = 95;
                          else if (alrm_tempF > 120)
                          alrm_tempF = 120;
                 
                             display.clearDisplay();
                             display.setTextSize(1);
                             display.setCursor(40, 0);
                             display.print("ALARM-F");
                             display.setTextSize(3);
                             display.setCursor(38, 10);
                             display.print(alrm_tempF, 1);
                             display.display();
                             
                                 if (!dowen)
                                   {
                                     delay(200);
                                     alrm_tempF--;
                                   }
                                 if(!upp)
                                   {
                                      delay(200);
                                      alrm_tempF++;
                                   }
                                 if (!enter)
                                   {
                                     delay(50);
                                     EEPROM.write(adres_2, alrm_tempF);
                                     break;
                                  }
                       }
                 }
        }
       break;
       case 2:
          {
                  if ((!selec))
                      {
                          delay(250);
                          count3++;
                          if (count3 > 1)
                          count3 = 0;
                          if (count3 == 0)
                           {
                                buzr_flg =0;
                                EEPROM.write(adres_1, buzr_flg);
                                display.clearDisplay();
                                display.setTextSize(2);
                                display.setCursor(7, 10);
                                display.print("BEEP ON");
                                display.display();
                                delay(500);
                           }
                          else
                           {
                                buzr_flg = 1;
                                EEPROM.write(adres_1, buzr_flg);
                                display.clearDisplay();
                                display.setTextSize(2);
                                display.setCursor(7, 10);
                                display.print("BEEP OFF");
                                display.display(); delay(500);
                            }

                      }
        }
         break;
          case 3:
          {
             display.clearDisplay();
             display.setTextSize(1);
             display.setCursor(0, 0);
             display.print("PASWRD SETINGS");
             display.setTextSize(2);
             display.setCursor(0, 10);
             display.print("PASSWORD");
             display.display();
             delay(5000);
            display.clearDisplay();
                  while(1)
                  {
                               
                                 password_set_decisoion();
                               
                         

                      }
        }
         break;
  }
  }
  }

 void calibraction_fn(void)
  {
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("calibration factor-F");
          display.setTextSize(2);
          display.setCursor(10, 10);
          display.print((char)237);
          display.print(" = ");
          display.print(cali_f,1);
          display.display();
          delay(1000);
     while(1)
      {
          button_cal();
           if(cali_f>=10.0)
             cali_f=-10.0;
             else if(cali_f<=-10.0)
             cali_f=10.0;
             
           if(!upp)
           {
             delay(200);
             cali_f=cali_f+0.1;
             
           }
           else if(!dowen)
           {
             delay(200);
             cali_f=cali_f-0.1;
             
           }
           if(!enter)
           break;
         
          EEPROM.put(adres_3, cali_f);
          display.clearDisplay();
          display.setTextSize(1);
          display.setCursor(0,0);
          display.print("calibration factor-F");
          display.setTextSize(2);
          display.setCursor(10, 10);
          display.print((char)237);
          display.print(" = ");
          display.print(cali_f,1);
          display.display();
         
    }
  }
void  password_check(void )
{
  int PASS_WORD_ENTER = 0, DEFAULT_PASSWORD = 1111, SET_PASS_WORD=0;
  delay(100);
 // SET_PASS_WORD|=EEPROM.read(addr15);
   SET_PASS_WORD=EEPROM.read(addr16)*256+EEPROM.read(addr15);
  PASS_WORD_ENTER= enter_pass_word();
  if (PASS_WORD_ENTER == SET_PASS_WORD ||PASS_WORD_ENTER == DEFAULT_PASSWORD)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(1,10);
    display.print("ACCESSED");
    display.setTextSize(2);
    display.display();
    delay(3000);
     setting_fn();
  }
  else {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(1,10);
    display.print("DENIED");
    display.setTextSize(2);
    display.display();
    delay(3000);
   
  }
}

//////////////////////////////////////////////////////enter password
int  enter_pass_word(void )
{
  byte pass_word_enter[4]={0,0,0,0}, dig_position = 0,indvdual_digits=0,last_byte=0,first_byte=0;
  int array_to_int = 0, w=0;
  while (1)
  {
    button_cal();
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2,1);
    display.print("enter_password:");
    display.setTextSize(2);
    display.setCursor(20, 12);
    display.print(pass_word_enter[0], 1);
    display.print(pass_word_enter[1], 1);
    display.print(pass_word_enter[2], 1);
    display.print(pass_word_enter[3], 1);
    display.setTextSize(2);
    display.setCursor(20+(dig_position*12),23);
    display.print('-');
    display.display();  
    delay(400);
    if (upp==0)
    {
      indvdual_digits++;
      delay(200);
      if (indvdual_digits >9)
      {
       indvdual_digits = 0;
        delay(200);
      }
     
       pass_word_enter[dig_position]=indvdual_digits;
   
    }
    else if (dowen==0)
    {
      dig_position++;
      delay(200);
      indvdual_digits=pass_word_enter[dig_position];
      if (dig_position> 3)
      {
        dig_position = 0;
        delay(200);
      }
    }
    else if (enter==0)
    {
      array_to_int = 0;
      for (int i = 0; i <= 3; i++)
      {
        array_to_int = pass_word_enter[i] + array_to_int * 10;
      }
      delay(200);
      return array_to_int;
    }

  }//1st while end
}
/////////////////////////////////////////password_reset()/////////////////////
void password_set(void)
{
  byte pass_word_enter[4]={0,1,2,3}, dig_position = 0,indvdual_digits=0,last_byte=0,first_byte=0;
  int array_to_int =0;
  while (1)
  {
     button_cal();
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("ENTER NEW PSWD:");
    display.setTextSize(2);
    display.setCursor(20, 12);
    display.print(pass_word_enter[0], 1);
    display.print(pass_word_enter[1], 1);
    display.print(pass_word_enter[2], 1);
    display.print(pass_word_enter[3], 1);
    display.setTextSize(2);
    display.setCursor(20+(dig_position*12),23);
    display.print('-');
    display.display();  
    if (upp==0)
    {
      indvdual_digits++;
      delay(200);
      if (indvdual_digits >9)
      {
       indvdual_digits = 0;
        delay(200);
      }
       pass_word_enter[dig_position]=indvdual_digits;  
    }
    else if (dowen==0)
    {  
      dig_position++;
      delay(200);
      indvdual_digits=pass_word_enter[dig_position];
      if (dig_position>3)
      {
        dig_position = 0;
        delay(200);
      }
    }
    else if (selec==0)
    {
      array_to_int = 0;
      for (int i = 0; i <= 3; i++)
      {
        array_to_int = pass_word_enter[i] + array_to_int * 10;
      }
      delay(200);
      last_byte=(array_to_int>>8);
      first_byte=(array_to_int);
    EEPROM.write(addr15,first_byte);  
    EEPROM.write(addr16,last_byte);
    setting_fn(); //GO MAIN LOOP
   
    }
}
}
///////////////////////////////////////////password_status//////////////////////////////
void password_set_decisoion(void )
{
 
  while (1)
  {button_cal();
       display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(2 , 1);
    display.print("password_set:-");
     display.setTextSize(2);
     display.setCursor(12,10);
     if(pass_word_status==0)
     {
      display.print("YES");
      }
      else if(pass_word_status==1)
      {
       display.print("NO");
      }
     display.display();
    if (upp==0)
    {
      pass_word_status++;
      delay(400);
      if (pass_word_status>1)
       pass_word_status=1;
    }
    else if (dowen==0)
    {
      pass_word_status--;
      delay(400);
      if (pass_word_status<0)
      pass_word_status=0;
    }
    else if (enter==0)
    {
      EEPROM.write(addr17,pass_word_status);
      delay(200);
      if(pass_word_status==0)
      {
      password_set();
      }
      setting_fn();
    }
     
  }
}
