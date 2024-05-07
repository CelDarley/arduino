
const int pinPwm1 = 3; // pino onde sai o sinal PWM
const int pinLer1 = A0; // pino que lê o sinal PWM

const int pinPwm2 = 5; // pino onde sai o sinal PWM
const int pinLer2 = A1; // pino que lê o sinal PWM

const int buzzer = 2; 

unsigned long previousMillis = 0; 
unsigned long previousMillis2 = 0; 
unsigned long previousMillis3 = 0; 


const long interval = 60; //4 HZ
const long interval2 = 180; //4 HZ

const int pinoAzul = 7;
const int pinoVermelho = 8;
 
//int pinoB = 9; 

bool alterado = false;

void setup() {
  Serial.begin(9600);

  pinMode(pinoAzul, OUTPUT); 
  pinMode(pinoVermelho, OUTPUT); 
//

  pinMode(buzzer, OUTPUT);
  
  pinMode(pinPwm1,OUTPUT); // Envia o sinal PWM
  pinMode(pinPwm2,OUTPUT); // Envia o sinal PWM
  
  pinMode(pinLer2,INPUT_PULLUP);  // lê o sinal Analógico
  pinMode(pinLer1,INPUT_PULLUP);  // lê o sinal Analógico
 
}
void pisca(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
    digitalWrite(pinoVermelho, !digitalRead(pinoVermelho)); 
  }
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    digitalWrite(pinoAzul, !digitalRead(pinoAzul));
  }
 }
void loop() {

  analogWrite(pinPwm1,128);// ENVIA EM GND
  analogWrite(pinPwm2,255); // ENVIA EM VCC
  
  int valorGnd = analogRead(pinLer1);// RECEBE DE GND
  int valorVCC = analogRead(pinLer2);// RECEBE DE VCC

  valorGnd = analogRead(pinLer1);
  valorGnd = map(valorGnd, 0, 1023, 0, 255);

  valorVCC = analogRead(pinLer2);
  valorVCC = map(valorVCC, 0, 1023, 0, 255);
  boolean tocarBuzzer=false;
  
  delay(150);
        // DESLIGADOS
         
        if(valorGnd > 250 ){
          Serial.println("gnd desligado");
          digitalWrite(pinoVermelho,LOW);
          tocarBuzzer=true;
        }
        if(valorVCC > 250){
          Serial.println("vcc desligado");
          digitalWrite(pinoAzul,LOW);
          tocarBuzzer=true;
        }
        
        // LIGADOS
        Serial.println(valorGnd); // 121
        if((valorGnd > 100)&&(valorGnd < 150)){
          digitalWrite(pinoAzul,HIGH);
          digitalWrite(buzzer, LOW);
        }
        Serial.println(valorVCC); // 72
        if((valorVCC > 60) && (valorVCC < 90)){
          digitalWrite(pinoVermelho,HIGH);
          digitalWrite(buzzer, LOW);
        }

        // INVERTIDOS 

        if((valorVCC > 100)&&(valorVCC < 150)){
          Serial.println("gnd INVERTIDO");
          tocarBuzzer=true;
          pisca();
        }
        if((valorGnd > 60) && ( valorGnd < 90)){
          Serial.println("vcc INVERTIDO");
          tocarBuzzer=true;
          pisca();
        }

      if(tocarBuzzer==true){
        digitalWrite(buzzer, HIGH);
      }
}
