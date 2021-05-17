int steer;
int vel=0;
float ki=0.001;
float kp=0.01;
float ki2=0.0001;
float kp2=0;
float vel2;
float error;
int prev;

void setup() {
  Serial.begin(19200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop(void) {
 int pot=analogRead(A3);
int veltar=map(pot,193,852,0,1000);
if(veltar<5)veltar=0;

  int ahora=millis();
  int dif=ahora-prev;
  error=veltar-vel2; 
  int integral = error*dif;



if(veltar>20)vel2=vel2+kp*error+ki*integral;
if(veltar<20)vel2=vel2+kp2*error+ki2*integral;
if(veltar>999)vel2=1000;
vel=vel2;
Serial.println(vel);
 //Serial.write((uint8_t *) &steer, sizeof(steer));
  //Serial.write((uint8_t *) &vel, sizeof(vel));
  delay(20);
  prev=ahora;
}
