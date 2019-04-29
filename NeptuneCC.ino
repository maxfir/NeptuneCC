/*
Name: NeptuneCC
Purpose: CruiseControl device for Malibu Echelon
Date: April 23, 2019
Designed and built by Ryan Maxfield
*/

#include <TinyGPS++.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

  TinyGPSPlus gps;
  LiquidCrystal_I2C lcd(0x27,20,4);
  float lat = 28.5458,lon = 77.1703;
  const int stepPin = 4;
  const int dirPin = 3;
  const int enPin = 2;
  double speedReduction = 0;
  double targetSpeed = .20;
  double realSpeed = 0;
  double speedDiff = 0;
  int maxRotate = 100;
  int minRotate = 0;
  int pos = 0;
  
void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  Serial1.begin(9600);
  Serial.begin(9600);
  lcdHello();
  gpsLock();
  lcd.clear();
  Serial.println("Setup Complete");
  motorStart();
}

void loop() {
  gpsData();
  actionCalc();
  lcdUpdate();
  //delay(1000);
}

void motorStart() {
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay

  digitalWrite(dirPin,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}

void actionCalc() {
  speedDiff = realSpeed - targetSpeed;
  Serial.println(speedDiff);
  if (speedDiff > 0 && pos < maxRotate){
    pos = pos + 1;
    digitalWrite(dirPin, LOW);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  else if (speedDiff < 0 && pos > minRotate){
    pos = pos - 1;
    digitalWrite(dirPin, HIGH);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
  else {
  }
}

void gpsData(){
  while(Serial1.available() > 0)
  gps.encode(Serial1.read());
  realSpeed = gps.speed.mph();
}

void lcdUpdate(){
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Target Speed =");
  lcd.setCursor(15,0);
  lcd.print(targetSpeed);
  lcd.setCursor(0,1);
  lcd.print("Real Speed =");
  lcd.setCursor(13,1);
  lcd.print(realSpeed);
  lcd.setCursor(0,2);
  lcd.print("Satellites =");
  lcd.setCursor(13,2);
  lcd.print(gps.satellites.value());
  displayReduction();
}

void displayReduction() {
  speedReduction = (pos / maxRotate);
  if (speedReduction <= 10) {
    lcd.setCursor(0,3);
    lcd.print("=                   ");
  }
    if (speedReduction > 10 && speedReduction <= 20) {
    lcd.setCursor(0,3);
    lcd.print(" =                  ");
  }
    if (speedReduction > 20 && speedReduction <= 30) {
    lcd.setCursor(0,3);
    lcd.print("  =                 ");
  }
    if (speedReduction > 30 && speedReduction <= 40) {
    lcd.setCursor(0,3);
    lcd.print("   =                ");
  }
    if (speedReduction > 40 && speedReduction <= 50) {
    lcd.setCursor(0,3);
    lcd.print("    =               ");
  }
    if (speedReduction > 50 && speedReduction <= 60) {
    lcd.setCursor(0,3);
    lcd.print("     =              ");
  }
    if (speedReduction > 60 && speedReduction <= 70) {
    lcd.setCursor(0,3);
    lcd.print("      =             ");
  }
    if (speedReduction > 70 && speedReduction <= 80) {
    lcd.setCursor(0,3);
    lcd.print("       =            ");
  }
    if (speedReduction > 80 && speedReduction <= 90) {
    lcd.setCursor(0,3);
    lcd.print("        =           ");
  }
    if (speedReduction > 90 && speedReduction <= 100) {
    lcd.setCursor(0,3);
    lcd.print("         =          ");
  }
  else{
  }
}

void lcdHello(){
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(7,1);
  lcd.print("Hello");
  delay (2000);
  lcd.setCursor(0,2);
  lcd.print("Acquiring Satellites");
}

void gpsLock() {
  while (realSpeed = 0){
    gpsData();
    delay(1000);
  }
  lcd.clear();
  lcd.setCursor(6,1);
  lcd.print("Hit it!");
  delay(2000);
}
