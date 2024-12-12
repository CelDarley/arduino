### nodes
#### buscando o mac

~~~bash
esptool.py --port /dev/ttyUSB0 read_mac 
~~~
c8:f0:9e:f1:74:94
ou 
0xC8, 0xF0, 0x9E, 0xF1, 0x74, 0x94

#### scketch pai
~~~arduino
#include <esp_now.h>
#include <WiFi.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

// Endereço MAC do segundo ESP32 (substitua pelo endereço correto)
uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0xF1, 0x74, 0x94};

// Estrutura da mensagem
typedef struct struct_message {
  char command;  // '1' para acordar, '0' para dormir
} struct_message;

struct_message myData;

void setup() {
  Serial.begin(115200);

  // Inicializa o Bluetooth
  if (!SerialBT.begin("ESP32_Controller")) {
    Serial.println(F("Erro blt"));
    return;
  }
  Serial.println(F("Aguard comand"));

  // Inicializa o Wi-Fi no modo Station
  WiFi.mode(WIFI_STA);
  Serial.println(F("Wi-Fi inic"));

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println(F("Erro ESP-NOW"));
    return;
  }

  // Registra o endereço do receptor
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println(F("Erro peer"));
    return;
  }
}

void loop() {
  if (SerialBT.available()) {
    char command = SerialBT.read();
    if (command == '1' || command == '0') {
      myData.command = command;
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      if (result == ESP_OK) {
        Serial.println(F("Comand ok"));
      } else {
        Serial.println(F("Erro c"));
      }
    }
  }
}~~~

#### scketch node filho
~~~arduino
#include <esp_now.h>
#include <WiFi.h>

// Estrutura da mensagem
typedef struct struct_message {
  char command;  // '1' para acordar, '0' para dormir
} struct_message;

struct_message receivedData;

// Variável para armazenar o endereço MAC do remetente
uint8_t senderMacAddress[6];

// Função de callback para receber dados
void onDataReceive(const esp_now_recv_info *info, const uint8_t *data, int data_len) {
  memcpy(&receivedData, data, sizeof(receivedData));
  memcpy(senderMacAddress, info->src_addr, 6);  // Armazena o endereço MAC do remetente

  if (receivedData.command == '1') {
    Serial.println("Acordar e começar a enviar dados");
    // Acordar e começar a enviar dados
  } else if (receivedData.command == '0') {
    Serial.println("Dormir e parar de enviar dados");
    // Dormir e parar de enviar dados
  }
}

void setup() {
  // Inicializa o Serial Monitor
  Serial.begin(115200);

  // Inicializa o Wi-Fi no modo Station
  WiFi.mode(WIFI_STA);
  Serial.println("Wi-Fi iniciado no modo Station");

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  // Registra a função de callback para receber dados
  esp_now_register_recv_cb(onDataReceive);
}

void loop() {
  if (receivedData.command == '1') {
    // Ler dados de uma porta analógica (GPIO 32)
    int sensorValue = analogRead(32);

    // Enviar dados de volta para o dispositivo 1
    esp_err_t result = esp_now_send(senderMacAddress, (uint8_t *)&sensorValue, sizeof(sensorValue));

    if (result == ESP_OK) {
      Serial.print("Dados enviados com sucesso: ");
      Serial.println(sensorValue);
    } else {
      Serial.println("Erro ao enviar dados");
    }

    delay(1000); // Envia dados a cada segundo
  }
}
~~~
