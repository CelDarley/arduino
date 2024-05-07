
#include <Adafruit_Fingerprint.h>

// Para placa UNO e outras com hardware serial
// pino #2 is entrada para sensor (GREEN wire)
// pino #3 is saida para arduino  (WHITE wire)

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\n teste de detecção Adafruit finger");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println(" encontrado fingerprint sensor!");
  } else {
    Serial.println("Não encontrou fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); 
  Serial.print(finger.templateCount); 
  Serial.println(" templates");
  Serial.println("aguardando pela validação...");
   pinMode(8, OUTPUT);
   digitalWrite(8,LOW);
   pinMode(10, OUTPUT);
   digitalWrite(10,LOW);
}

void loop()                     
{ 
    getFingerprintIDez();
    delay(50);            

}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem captada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Imagem não detectada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de Comunicação");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro na Imagem");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_FEATUREFAIL:
    Serial.println("Nao pode localizar fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Nao pode localizar fingerprint features");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Encontrou um print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicação");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Não deu Mach");
    return p;
  } else {
    Serial.println("Erro desconhecido");
    return p;
  }   
  
  // found a match!
  Serial.print("Encontrou ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  digitalWrite(8,HIGH);
  digitalWrite(10,HIGH);
  delay(10000);
  digitalWrite(8,LOW);
  digitalWrite(10,LOW);
  return finger.fingerID; 
}
