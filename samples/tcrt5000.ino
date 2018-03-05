#define SENSOR_PIN 31
int objeto = 0;  
   
void setup()  
{  
  pinMode(SENSOR_PIN, INPUT); //Pino ligado ao coletor do fototransistor  
  Serial.begin(9600);  
}  
   
void loop()  
{  
  objeto = digitalRead(SENSOR_PIN);  
  if (objeto == 0)  
  {  
    Serial.println("Objeto : Detectado");  
  }  
  else  
  {  
    Serial.println("Objeto : Ausente !");  
  }  
}
