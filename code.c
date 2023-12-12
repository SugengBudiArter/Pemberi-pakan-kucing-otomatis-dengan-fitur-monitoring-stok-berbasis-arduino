#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define trigpin 7
#define echopin 6
#define servoPin 9

Servo servo;

const int maxDistance = 100; 
const int minDistance = 20;  
const int servoOpenAngle = 90;
const int servoCloseAngle = 0;
const unsigned long feedDuration = 5000;

unsigned long lastFeedTime = 0;

void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(servoCloseAngle);
  lcd.backlight();
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
}

void drawProgressBar(int percentage) {
  int numOfBlocks = map(percentage, 0, 100, 0, 16);
  
  lcd.setCursor(0, 1);
  lcd.print("");
  for (int i = 0; i < 16; ++i) {
    if (i < numOfBlocks) {
      lcd.print("=");
    } else {
      lcd.print(" ");
    }
  }
  lcd.print("");
}

void loop(){
  if (millis() - lastFeedTime >= feedDuration) {
    servo.write(servoOpenAngle);
    delay(3000); 
    servo.write(servoCloseAngle);
    lastFeedTime = millis();
  }

  long duration, distance;
  
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  
  duration = pulseIn(echopin, HIGH);
  distance = (duration / 2) * 0.03448;
  
  int percentage = map(distance, minDistance, maxDistance, 100, 0);
  percentage = constrain(percentage, 0, 100);
  
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Stok Pakan: ");
  lcd.print(percentage);
  lcd.print("%");
  
  drawProgressBar(percentage);
 
  delay(1000);
}
