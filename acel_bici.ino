float salida;
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, OUTPUT);
 
}

// the loop function runs over and over again forever
void loop() {
  float acelerador=analogRead(A3);
  float acelerador_esc=(acelerador*5)/1024;
 
  //float salida_esc=(0.1*pow(acelerador_esc,2)+log(acelerador_esc)+0.8);
  float salida_esc=(0.07*pow(acelerador_esc,2)+0.2*(acelerador_esc)+0.6);
  if(acelerador_esc>3.50){salida_esc=acelerador_esc;}
  salida=salida_esc*255/5;
  
  
  
   if (salida <40)
    {
      salida=40;
    }else if (salida>255)
    {
      salida=255;
    }  
   analogWrite(10, salida); 
   //Serial.print(salida);
  //Serial.print("   voltios entrada:");
  //Serial.print(acelerador_esc);
  //Serial.print("  voltios salida:");
  //Serial.println(salida_esc);                  
}
