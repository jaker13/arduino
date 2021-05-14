#include <Wire.h>
#include "RTClib.h"

byte vhuerto=2;
byte varbolado=3;
byte vjardin=4;
byte vagua=5;
byte bomba=8;
//A0 = caudalimetro
//A1 = nivel de agua
int nivel=0;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------VALORES MODIFICABLES--------------------------------------------------------------------------
int hora_arranque=23;   //hora en la que comienza el riego
int min_arranque=21;    //minuto en el que comienza el riego
int lim1[12]={5,5,5000,5000,5,5001,50,5000,50,500,50,50};         //valor en litros por meses para huerto {enero, febrero, ...}
int lim2[12]={5000,50,50,50,5000,5000,5000,5000,5000,5000,5000,5000}; //valor en litros por meses para arbolado {enero, febrero, ...}
int lim3[12]={5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000};  //valor en litros por meses para jardin {enero, febrero, ...}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------


RTC_DS3231 rtc;

String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre" };

void setup() {
  
   pinMode(vhuerto, OUTPUT);
   pinMode(varbolado, OUTPUT);
   pinMode(vjardin, OUTPUT);
   pinMode(vagua, OUTPUT);
   pinMode(bomba, OUTPUT);
   digitalWrite(vhuerto, LOW);
   digitalWrite(bomba, LOW);
   
   Serial.begin(115200);
   delay(100); 

   if (!rtc.begin()) {
      Serial.println(F("RTC no ok"));
      while (1);
   }

   // Si se ha perdido la corriente, fijar fecha y hora
   if (rtc.lostPower()) {
      // Fijar a fecha y hora de compilacion
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   }
}

void riego(int lim){
  float can;
  float flow;
  
  while(can<lim){
  flow=analogRead(A0);
  Serial.println(can);
  can=can+flow*30.0/(60.0*1024.0);
  delay(1000); 
  }
  Serial.println("comp");
}

void llenar_cuba(){
  nivel=analogRead(A1);
  if(nivel<150){
    while(nivel<800){
      nivel=analogRead(A1);
      digitalWrite(vagua,HIGH);
    }
    digitalWrite(vagua,LOW);
  }
}

void loop() {
   DateTime date = rtc.now();
   llenar_cuba();  
   if(date.hour()==hora_arranque && (date.minute())==min_arranque){
      digitalWrite(vhuerto, HIGH);
      digitalWrite(bomba, HIGH);
      llenar_cuba();
      riego(lim1[date.month()-1]);     
      digitalWrite(varbolado, HIGH);
      digitalWrite(vhuerto, LOW);
      llenar_cuba();
      riego(lim2[date.month()-1]);
      digitalWrite(vjardin, HIGH);
      digitalWrite(varbolado, LOW);
      llenar_cuba();
      riego(lim3[date.month()-1]);
      digitalWrite(bomba, LOW);
      digitalWrite(varbolado, LOW);
   }
   delay(3000);
}
