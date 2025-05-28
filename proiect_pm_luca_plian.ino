#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
#include <Time.h>
#include <TimeLib.h>
DS3232RTC myRTC;

float minTemperature = 200.0;
float maxTemperature = -200.0;




Servo myservo;
int occurancesElementZero = 0;
int occurancesElementZero2 = 0;

int SilentMode = 0;
int LCD_Display_Mode = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define BTN_PIN1 4
#define BTN2_PIN 7

#define ONE_WIRE_BUS 8
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11
#define BUZZER A1
#define tonePin A1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int DC=3;

float Celsius = 0;
float Fahrenheit = 0;



void setup() {
  pinMode(12, OUTPUT);
  pinMode(BTN_PIN1, INPUT_PULLUP); 
  pinMode(BTN2_PIN, INPUT_PULLUP); 

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  sensors.begin();

  Serial.begin(9600);
  myservo.attach(6);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  pinMode(DC, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(DC, LOW);
  Serial << F( "\n" __FILE__ "\n" __DATE__ " " __TIME__ "\n" );
  myRTC.begin();

  setSyncProvider(myRTC.get);
  Serial << F("RTC Sync");
  if (timeStatus() != timeSet) Serial << F(" FAIL!");
  Serial << endl;

}
void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);

}
void printI00(int val, char delim)
{
    if (val < 10) lcd << '0';
    lcd << _DEC(val);
    if (delim > 0) lcd << delim;
    return;
}

void printDate(time_t t)
{
    printI00(day(t), 0);
    lcd << " ";

    lcd << monthShortStr(month(t)) <<" "<< _DEC(year(t));
}
void loop() {

  time_t t;

  sensors.requestTemperatures();
  Celsius = sensors.getTempCByIndex(0);
  digitalWrite(12, HIGH);

  lcd.setCursor(0, 0);
  int16_t roundedCelsius = Celsius * 10;
  int pressed = digitalRead(BTN_PIN1);
  int pressed2 = digitalRead(BTN2_PIN);

  t = now();



  
  
  int old_occurances = occurancesElementZero; 
  if (pressed2 == 0){
    occurancesElementZero++;
  } else{
    occurancesElementZero = 0;
  }
  if (occurancesElementZero==1 and old_occurances==0){

    if (SilentMode==0) { 
    SilentMode = 1;
    } else{
       SilentMode = 0;
    }
  }

  int old_occurances2 = occurancesElementZero2; 
  
  if (pressed == 0){
    occurancesElementZero2++;
  } else{
    occurancesElementZero2 = 0;
  }
  if (occurancesElementZero2==1 and old_occurances2==0){

    if (LCD_Display_Mode==0) { 
    LCD_Display_Mode = 1;
    } else{
      LCD_Display_Mode = 0;
    }
  }

 

  if (LCD_Display_Mode==0){
    lcd.print("Temp. : ");
    lcd.print((float)roundedCelsius/10, 1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.print("         ");
    lcd.setCursor(0, 1);
  } else{
    lcd.setCursor(-2, 0);
    lcd.print("Data:");
    printDate(t);
    lcd.print("     ");
    lcd.setCursor(-2, 1);
    lcd.print("Max:");
    lcd.print(int(maxTemperature));
    lcd.print((char)223);
    lcd.print("C");
    lcd.print("Min:");
    lcd.print(int(minTemperature));
    lcd.print((char)223);
    lcd.print("C");
    
    
    

  }

  if (hour(t) == 0 and minute(t) == 0){
    minTemperature = 200;
    maxTemperature = -200;
  }


  if (minTemperature > (float)roundedCelsius/10 ){
    minTemperature = (float)roundedCelsius/10;
  }

  if (maxTemperature < (float)roundedCelsius/10 ){
    maxTemperature = (float)roundedCelsius/10;
  }


  if ((float)roundedCelsius/10 < 0.0){
    myservo.write(0);   
  } else if ((float)roundedCelsius/10 >= 60.0){
    myservo.write(180);
  } else{
    myservo.write((float)(roundedCelsius/10)*(18.0/7.0));
  }


  if (roundedCelsius/10 < 15.0){
    if (LCD_Display_Mode==0) {
      lcd.print("Este frig");
      setColor(255, 0, 0);
      lcd.print("         ");
      digitalWrite(DC, LOW);
    }
  

  } else if (roundedCelsius/10 < 35.0){
    if (LCD_Display_Mode==0) {
      lcd.print("Este bine");
      setColor(0, 255, 0);
      lcd.print("         ");
      digitalWrite(DC, LOW);
    }
    

  } else{
    if (LCD_Display_Mode==0) {
      lcd.print("Este cald");
      setColor(0, 0, 255);
      lcd.print("         ");
      digitalWrite(DC, HIGH);
    
    }
    if (!SilentMode){
    tone(BUZZER, 85);  
    delay(1000);   
    noTone(BUZZER);   
    delay(1000);
    }
    digitalWrite(DC, HIGH);

  }

  delay(1000);

}