
const int pinLer1 = A0; // pino que lê o sinal PWM
const int pinLer2 = A1; // pino que lê o sinal PWM
const int pinLer1RX = A2; // pino que lê o sinal PWM
const int pinLer2TX = A3; // pino que lê o sinal PWM

const int pinPwm1 = 3; // pino onde sai o sinal PWM
const int pinPwm1RX = 4; // pino onde sai o sinal PWM para RX (BR)
const int pinPwm2 = 5; // pino onde sai o sinal PWM
const int pinPwm2TX = 6; // pino onde sai o sinal PWM para TX (AZ)

const int buzzer = 2; 

unsigned long previousMillis = 0; 
unsigned long previousMillis2 = 0; 
unsigned long previousMillis3 = 0; 


const long interval = 60; //4 HZ
const long interval2 = 180; //4 HZ

const int pinoAzul = 7;
const int pinoVermelho = 8;
const int ledRX = 9;
const int ledTX = 10;

bool alterado = false;

void setup() {
  Serial.begin(9600);

  pinMode(pinoAzul, OUTPUT); 
  pinMode(pinoVermelho, OUTPUT); 
  pinMode(ledRX, OUTPUT); 
  pinMode(ledTX, OUTPUT);
//

  pinMode(buzzer, OUTPUT);
  
  pinMode(pinPwm1,OUTPUT); // Envia o sinal PWM
  pinMode(pinPwm2,OUTPUT); // Envia o sinal PWM
  pinMode(pinPwm1RX,OUTPUT); // Envia o sinal PWM para fio RX (BR)
  pinMode(pinPwm2TX,OUTPUT); // Envia o sinal PWM para fio TX (AZ)
  
  pinMode(pinLer1,INPUT_PULLUP);  // lê o sinal Analógico
  pinMode(pinLer2,INPUT_PULLUP);  // lê o sinal Analógico
  pinMode(pinLer1RX,INPUT_PULLUP);  // lê o sinal Analógico para fio RX (BR)
  pinMode(pinLer2TX,INPUT_PULLUP);  // lê o sinal Analógico para fio TX (AZ)
  
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
  analogWrite(pinPwm1RX,128);// ENVIA EM RX
  analogWrite(pinPwm2TX,255); // ENVIA EM TX
  
  int valorGnd = analogRead(pinLer1);// RECEBE DE GND
  int valorVCC = analogRead(pinLer2);// RECEBE DE VCC
  int valorRX = analogRead(pinLer1RX);// RECEBE DE GND
  int valorTX = analogRead(pinLer2TX);// RECEBE DE VCC

  valorGnd = map(valorGnd, 0, 1023, 0, 255);
  valorVCC = map(valorVCC, 0, 1023, 0, 255);
  valorRX = map(valorRX, 0, 1023, 0, 255);
  valorTX = map(valorTX, 0, 1023, 0, 255);
  
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
        if(valorRX > 250 ){
          Serial.println("RX desligado");
          digitalWrite(ledRX,LOW);
          tocarBuzzer=true;
        }
        if(valorTX > 250){
          Serial.println("TX desligado");
          digitalWrite(ledTX,LOW);
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
        Serial.println(valorRX); // 121
        if((valorRX > 100)&&(valorRX < 150)){
          digitalWrite(ledRX,HIGH);
          digitalWrite(buzzer, LOW);
        }
        Serial.println(valorTX); // 72
        if((valorTX > 60) && (valorTX < 90)){
          digitalWrite(ledTX,HIGH);
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
        if((valorRX > 100)&&(valorRX < 150)){
          Serial.println("RX INVERTIDO");
          tocarBuzzer=true;
          pisca();
        }
        if((valorTX > 60) && ( valorTX < 90)){
          Serial.println("TX INVERTIDO");
          tocarBuzzer=true;
          pisca();
        }
      if(tocarBuzzer==true){
        digitalWrite(buzzer, HIGH);
      }
}
