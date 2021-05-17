#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16,2);

unsigned char data[57];
unsigned int volt;
byte residuo;

float voltage;
float vel=0.0;
float velmax;
float bat;
float bat_aux;
float trip;
float trip_1;
float odo;
float vmin;
float vmax;

int p_seg=0;
int pag;
int modo=0;
int porbat;
int secreto;

long int ahora;
long int tiempo;
long int tiempo_reset;
long int cuenta=0;
long int start=0;

bool mant;
bool mact;
bool posact;
bool posant;
bool negant;
bool negact;
bool comm;
bool emergencia;
bool m1_ack;
bool m2_ack;
bool m3_ack;


void reqeust_data(void){
  
  // request data for the controller
  Serial1.write((byte)0x01);
  delay(2);
  Serial1.write((byte)0x03);
  delay(2);
  Serial1.write((byte)0x0A);
  delay(2);
  Serial1.write((byte)0xBC);
  delay(2);
  Serial1.write((byte)0x00);
  delay(2);
  Serial1.write((byte)0x1A);
  delay(2);
  Serial1.write((byte)0x06);
  delay(2);
  Serial1.write((byte)0x3D);
  delay(2);
  tiempo=millis();
}


void enable_data(void){
  
  // enable data for the controller ones on power up
  Serial1.write((byte)0x01);
  delay(2);
  Serial1.write((byte)0x06);
  delay(2);
  Serial1.write((byte)0x0F);
  delay(2);
  Serial1.write((byte)0xC7);
  delay(2);
  Serial1.write((byte)0x34);
  delay(2);
  Serial1.write((byte)0x21);
  delay(2);
  Serial1.write((byte)0xED);
  delay(2);
  Serial1.write((byte)0xFB);
  delay(2);
}

void boost(void) {
  Serial.println("Entering Boost Mode!");
  //SET Speed limit
  Serial1.write((byte)0x01);
  delay(2);
  Serial1.write((byte)0x06);
  delay(2);
  Serial1.write((byte)0x0A);
  delay(2);
  Serial1.write((byte)0x4C);
  delay(2);
  Serial1.write((byte)0x00);
  delay(2);
  Serial1.write((byte)0x64); //0A= 10%, 0x64 = 100%
  delay(2);

  //CRC
  Serial1.write((byte)0x4A);
  delay(2);
  Serial1.write((byte)0x2E);
  delay(2);

  delay(200);
  while (Serial1.available()) { // dump response
    Serial.write(Serial1.read());
  }

  //SET Power limit
  Serial1.write((byte)0x01);
  delay(2);
  Serial1.write((byte)0x06);
  delay(2);
  Serial1.write((byte)0x09);
  delay(2);
  Serial1.write((byte)0xF7);
  delay(2);
  Serial1.write((byte)0x00);
  delay(2);
  Serial1.write((byte)0x50); //0A = 10A, 0x50 = 80A
  delay(2);

  //CRC
  Serial1.write((byte)0x3B);
  delay(2);
  Serial1.write((byte)0x98);
  delay(2);

  delay(200);
  while (Serial1.available()) { // dump response
    Serial.write(Serial1.read());
  }
  m3_ack=1;
}

void legal(void) {
  Serial.println("Entering Legal Compliance Mode!");
  //SET Speed limit
  Serial1.write((byte)0x01);
  delay(2);
  Serial1.write((byte)0x06);
  delay(2);
  Serial1.write((byte)0x0A);
  delay(2);
  Serial1.write((byte)0x4C);
  delay(2);
  Serial1.write((byte)0x00);
  delay(2);
  Serial1.write((byte)0x0A); //0A = 10%, 0x64 = 100%
  delay(2);

  //CRC
  Serial1.write((byte)0xCB);
  delay(2);
  Serial1.write((byte)0xC2);
  delay(2);

  delay(200);
  while (Serial1.available()) { // dump response
    Serial.write(Serial1.read());
  }

  //SET Power limit
  Serial1.write((byte)0x01);
  delay(2);
  Serial1.write((byte)0x06);
  delay(2);
  Serial1.write((byte)0x09);
  delay(2);
  Serial1.write((byte)0xF7);
  delay(2);
  Serial1.write((byte)0x00);
  delay(2);
  Serial1.write((byte)0x0A); //0A = 10A, 0x96 = 150A
  delay(2);

  //CRC
  Serial1.write((byte)0xBB);
  delay(2);
  Serial1.write((byte)0xA3);
  delay(2);

  delay(200);
  while (Serial1.available()) { // dump response
    Serial.write(Serial1.read());
  }
}


void pantalla0 (void){
  lcd.setCursor(0, 0);
  lcd.print(voltage);
  lcd.print("V   ");
  lcd.print(vel);
  lcd.print("kmh ");
  lcd.setCursor(0, 1);
  lcd.print(bat);
 lcd.print("V  M");
 lcd.print(modo);
  lcd.setCursor(13, 1);
 if(comm==0)lcd.print(" ");
 if(comm==1)lcd.print("*");
 if(emergencia==1)lcd.print("@");
 if(emergencia==0)lcd.print(" ");
}
void pantalla1(){
   lcd.setCursor(0, 0);
  lcd.print("max: ");
  lcd.print(velmax);
  lcd.print("KMh");
  lcd.setCursor(0, 1);
  lcd.print("TRIP: ");
  lcd.print(trip);
  lcd.print("km");
}
void pantalla2(){
   lcd.setCursor(0, 0);
  lcd.print("ODO ");
  lcd.print(odo);
  lcd.print("KM  ");
  lcd.print(secreto);
  lcd.setCursor(0, 1);
  lcd.print("TRIP_1:");
  lcd.print(trip_1);
   lcd.print("KM  ");
  
}

void pantalla3(){
   lcd.setCursor(0, 0);
  //lcd.print(" ");
  lcd.print(porbat);
  lcd.print("% Vmin");
  lcd.print(vmin);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Vmax");
  lcd.print(vmax);
  
}

void pulso(){
  cuenta++;
}

void setup() {
  EEPROM.get(0,odo);
  EEPROM.get(10,trip_1);
  Serial.begin(115200);
  Serial1.begin(19200);
  lcd.init();
  lcd.backlight();
  pinMode(11,INPUT_PULLUP); //BOTON POWER
  pinMode(10,INPUT_PULLUP); //BOTON +
  pinMode(9,INPUT_PULLUP);  //BOTON -
  pinMode(8,INPUT_PULLUP);  //BOTON PAG
  pinMode(7,OUTPUT); //pin7 salida rele conectar bateria auxiliar a medicion
  pinMode(2,INPUT_PULLUP);
  //attachInterrupt (0, pulso, RISING);
  digitalWrite(7, LOW);
  Serial.println("HMI SABVOTON");
  Serial.println(" ");
  delay(3000);
  enable_data();
  delay(400);
   if(Serial1.available()==8){ //wait for the data
    for(int i=0;i<8;i++){
      data[i] =  Serial1.read(); // put data in a array
      Serial.println(data[i],HEX);
    }
   }
  delay(20);
  legal();
  delay(20);
   mant=1;
   emergencia=1;

  // while(digitalRead(10)==1){
    //lcd.setCursor(0, 0);
    //lcd.print("PASSWORD");
   //}
   
}

void loop() {
  
  //envio de datos de modo
  if(modo==3 and emergencia==0 and m3_ack==0) boost();
  
  //resetear ack de modos
  if(modo!=2)m2_ack=0;
  if(modo!=3)m3_ack=0;
  
  if(millis()-ahora>200){
    reqeust_data();
    ahora=millis();
  }
  delay(220); //espera respuesta
  
  
  //seleccion pantalla
  if(pag==0)pantalla0();
  if(pag==1)pantalla1();
  if(pag==2)pantalla2();
  if(pag==3)pantalla3();
  
  //flancos botones
  posact=digitalRead(10);
  negact=digitalRead(9);
  mact=digitalRead(8);
  if(mact==1 and mant==0){
    pag++;
    lcd.clear();
    if(pag==4)pag=0; //limite de paginas
  }
 mant=mact;


if(posact==1 and posant==0){
    if(pag==0 and emergencia==0)modo++;
    if(pag==2)secreto++; 
    if(secreto==10)secreto=0; 
    if(modo==6)modo=5;
  }
  posant=posact;
 
if(negact==1 and negant==0){
    if(pag==0)modo--;
    if(modo==-1)modo=0;
    if(pag==1){trip=0.0; velmax=0.0;}
    if(pag==2)trip_1=0.0;
    
  }
  negant=negact;
  

  
  if(Serial1.available()==57){ //wait for the data
    for(int i=0;i<57;i++){
      data[i] =  Serial1.read(); // put data in a array
      //Serial.println(data[i],HEX);
    }
    voltage = (((unsigned int)data[5] << 8) + data[6]) /54.6;
    int torque = (((unsigned int)data[13] << 8) + data[14]);
    int torquecmd = (((unsigned int)data[11] << 8) + data[12]);
    int estado = data[4];
    int nbit=data[2];
    int mvel=data[22];
    float acel=(((unsigned int)data[25] << 8) + data[26]) /7663.0;
    Serial.print("voltage = ");
    Serial.print(voltage,2);
    Serial.print(" mvel:");
    Serial.print(mvel);
    Serial.print("torque ");
    Serial.print(torque);
    Serial.print(" torcmd:");
    Serial.print(torquecmd);
    Serial.print(" stado:");
    Serial.print(estado);
    Serial.print(" acel:");
    Serial.print(acel);
   
    Serial.println(" ");
  }

  //lectura baterias
  bat=analogRead(A0);
  bat=(bat*0.004883)*18.35;
  if(bat<vmin)vmin=bat;
  if(bat>vmax)vmax=bat;
  porbat=(bat-64)*100/(84.4-64);
  bat_aux=analogRead(A1);    
  bat_aux=(bat*0.004883)*10;
  
  
  //calculo velocidad+trip+odo
  if((millis()-start)>500){    
    vel=cuenta*2.2*3.6;
    trip=trip+(cuenta*0.0022);
    trip_1=trip_1+(cuenta*0.0022);
    odo=odo+(cuenta*0.0022);
    EEPROM.put(0, odo);
    EEPROM.put(10, trip_1);
    start=millis();
   cuenta=0;
  }
  //calculo vel max
  if(vel>velmax)velmax=vel;
 
  
  
   comm= not comm; //heartbit
   //modo limitado contraseña=4 en pagina2
   if(secreto==4)emergencia=0;
   if(secreto!=4)emergencia=1;
   
   //borrar buffer si excede de 57 caracteres
   
 Serial.println(Serial1.available());
    Serial.println(comm);
   
}
