#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  //A5 scl, A4 sda

#define DHTTYPE DHT11   // DHT 11
const int DHTPin = 5;     // what digital pin we're connected to
DHT dht(DHTPin, DHTTYPE);


int temperatura;
int cal;
float seltemp;
float h;

int pot;
long tiempo;
long tluz;
int reloj;
int potant;
int req;

void pantalla(void){

lcd.setCursor(0,0);
lcd.print("T:");
lcd.print(temperatura);
lcd.print("oC ");
lcd.setCursor(7,0);
lcd.print("H:");
lcd.print(h);
lcd.print("% ");
lcd.setCursor(0,1);
lcd.print("SET:");
lcd.print(seltemp);
lcd.setCursor(9,1);
lcd.print("oC");
lcd.setCursor(13,1);
lcd.print(req);
if(req<10)lcd.print(" ");
lcd.setCursor(15,1);
if(cal==1)lcd.print("*");
if(cal==0)lcd.print(" ");
/*
if(req=!potant){ //tiempo de apagado pantalla
  potant=req;
  tluz=millis();
  lcd.display();
}
//if(millis()-tluz>5000)lcd.noDisplay();
*/
}


void setup() {
lcd.init();
dht.begin();
Serial.begin(19200);
pinMode(4,OUTPUT);
digitalWrite(4,LOW);
pinMode(2,OUTPUT);
digitalWrite(2,LOW);
pinMode(3,OUTPUT);
digitalWrite(3,HIGH);
lcd.backlight();
tiempo=millis();
}

void loop() {

if(millis()-tiempo>1000){
  reloj=reloj+1;
  tiempo=millis();
  h = dht.readHumidity();
  temperatura = dht.readTemperature();
}
pot=analogRead(A6);
req=map(pot,0,1023,0,20);
float seltempm=map(pot,0,1023,1700,3000);
seltemp=seltempm/100;
if(reloj<req){
  digitalWrite(4,HIGH);
  cal=1;
}
if(reloj>req){
  digitalWrite(4,LOW);
  cal=0;
}
if(reloj==20){
  reloj=0;
}



digitalWrite(13,cal);
pantalla();

}
