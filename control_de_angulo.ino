byte ENABLE=8;
byte A1DIR=6;

byte A1STEP=3;

int A1VEL=0;

int A1VEL_INSTANTANEA=0;
float A1POS=0.0;

float A1TAR=0.0;

float A1DIFF=0.0;
float A1MICROSTEPPING=200.0;
int i;
int j;
int acc;
long ant;
long act;

void setup() {
  Serial.begin(115200);
 pinMode(ENABLE, OUTPUT); //enable
 pinMode(A1DIR, OUTPUT);
 pinMode(A1STEP, OUTPUT);
 
A1VEL=500;
acc=1;
A1VEL_INSTANTANEA=1000;
A1TAR=0.0;
}

void loop() {
 act=millis();
 digitalWrite(ENABLE, LOW);
 if (Serial.available()) {
  String str = Serial.readStringUntil('\n');
      float data = str.toFloat();
   A1TAR=data;
    A1VEL_INSTANTANEA=1000;
 }

 if(A1VEL_INSTANTANEA>=A1VEL) A1VEL_INSTANTANEA = A1VEL_INSTANTANEA-acc; //ACELERACION
 if(A1VEL_INSTANTANEA<1) A1VEL_INSTANTANEA=1;
  
  A1DIFF=A1POS-A1TAR;
  if((A1POS-A1TAR)>0) digitalWrite(A1DIR, LOW);
  if((A1POS-A1TAR)<0) digitalWrite(A1DIR, HIGH);
  
 
 if(A1DIFF>2 || A1DIFF<-2){
  digitalWrite(A1STEP, LOW);
  delayMicroseconds(A1VEL_INSTANTANEA);
  digitalWrite(A1STEP, HIGH);
  delayMicroseconds(A1VEL_INSTANTANEA);
  if((A1POS-A1TAR)<0)A1POS=A1POS+360.0/A1MICROSTEPPING;
  if((A1POS-A1TAR)>0)A1POS=A1POS-360.0/A1MICROSTEPPING;
 }
 
 if(millis()-ant>1000){
 Serial.println(A1POS);
 Serial.print(" ");
 Serial.print(A1TAR);
 Serial.print(" ");
 Serial.println(A1DIFF);
 ant=millis();

  }
 
}
