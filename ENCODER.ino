const int intPin = 2;
const int nema = 12;
int tiempo=0;
long ISRCounter=0;
double velenc=0.0;
long act=0;

double k=1.49;
int pot;

// rpm nema (1/(2*tiempo/10^6)/3600)*60 // tiempo=10 >> 800rpm
//rpm encoder counter*(1000/t)/200*60

void setup() {
//Serial.begin(115200);
pinMode(intPin, INPUT_PULLUP);
pinMode(nema, OUTPUT); 
attachInterrupt(digitalPinToInterrupt(intPin), encoder, RISING);
act=millis();
}

void loop() {
  
  if(millis()-act>100){
    velenc=ISRCounter*(1000.0/(millis()-act))/200.0;  //rps
    ISRCounter=0;
    tiempo=k*12000.0/velenc;
    act=millis();
    //Serial.println(tiempo);
    //pot=analogRead(A1);
    //k=map(pot,0,1023,20,300)/100.0;
    
  }
  //if(tiempo>salida && salida<2500)salida=salida+1;
  //if(tiempo<salida && tiempo>100)salida=salida-1;
  if(tiempo<10)tiempo=0;
  if(tiempo>10 && tiempo<200)tiempo=200;
  if(tiempo>100 && tiempo<5500){
  digitalWrite(nema, HIGH);
   delayMicroseconds(tiempo);
   digitalWrite(nema, LOW);
   delayMicroseconds(tiempo);
  }
  
}

void encoder()
{
   ISRCounter++;
}
