#include<SoftwareSerial.h>
SoftwareSerial Serial1(2,3);
#include<LiquidCrystal.h>
LiquidCrystal lcd(14,15,16,17,18,19);
int flag=0;
String str="";
#define motor 11
#define sensor 7
void setup()
{
  lcd.begin(16,2);
  Serial1.begin(9600);
  Serial.begin(9600);
  pinMode(motor, OUTPUT);
  pinMode(sensor, INPUT_PULLUP);
  lcd.print("Checking Status");
  lcd.setCursor(4,1);
  delay(2000);
  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
}
void loop()
{
    lcd.setCursor(0,0);
    lcd.print("System Working    ");
    if(digitalRead(sensor)==1 && flag==0)
    {
      delay(1000);
      if(digitalRead(sensor)==1)
      { 
        lcd.begin(16,2);
        lcd.setCursor(0,1);
        lcd.print("Motor ON    ");
        delay(2000);
        digitalWrite(motor, HIGH);
        flag=1;
        gsmInit();
        sendSMS("Low Soil Moisture detected.motor is on");
      }
    }
    else if(digitalRead(sensor)==0 && flag==1)
    {
      delay(1000);
      if(digitalRead(sensor)==0)
      {
        lcd.begin(16,2);
        lcd.setCursor(0,1);
        lcd.print("Motor OFF   ");
        delay(2000);
        digitalWrite(motor, LOW);
        flag=0;
        gsmInit();
        sendSMS("Soil Moisture is Normal.motor is off");
      }
    }
}
 
void sendSMS(String msg)
{
  lcd.clear();
  lcd.print("MSG. commands");
  Serial1.println("AT+CMGF=1");
  delay(500);
  Serial1.print("AT+CMGS=");
  Serial1.print('"');
  Serial1.print("+91*******");    // number
  Serial1.print('"');
  Serial1.println();
  delay(500);
  Serial1.println(msg);
  delay(500);
  Serial1.write(26);
  delay(1000);
  lcd.clear();
  lcd.print("System Status");
  delay(2000);
  lcd.begin(16,2);
}
void gsmInit()
{
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
    Serial1.println("AT");
    while(Serial1.available()>0)
    {
      if(Serial1.find("OK"))
      {
      at_flag=0;
      }
      else 
      {
        lcd.clear();
        lcd.print("Module Absent");
        delay(1000);
        return;
      }
    }
  }
  Serial1.println("ATE0");
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    Serial1.println("AT+CPIN?");
    while(Serial1.available()>0)
    {
      if(Serial1.find("READY"))
     { 
      net_flag=0;
      break;
     }
      else
      {
        lcd.clear();
        lcd.print("Network Absent");
        delay(1000);
        break;
      }
    }
    break;
  }
   Serial1.println("AT+CNMI=2,2,0,0,0");
   delay(1000);
   Serial1.println("AT+CMGF=1");
   delay(1000);
   Serial1.println("AT+CSMP=17,167,0,0");
   lcd.clear();
   Serial1.flush();
}
