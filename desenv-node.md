### nodes
#### buscando o mac

~~~bash
esptool.py --port /dev/ttyUSB0 read_mac 
~~~
c8:f0:9e:f1:74:94
ou 
0xC8, 0xF0, 0x9E, 0xF1, 0x74, 0x94

#### scketch pai
~~~c++
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
}

~~~

#### scketch node filho
~~~c++
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
###  Agora no modelo para comunicar Raspberry PI com esp 32 (O Rasp é o Pai e os esp32 são os filhos. )

#### scketch do filho

~~~c++
#include <esp_now.h>
#include <WiFi.h>

// Variável para controlar o estado do dispositivo
bool isAwake = false;  // Certifique-se de que esta variável está declarada no escopo global

// Estrutura da mensagem
typedef struct struct_message {
  char command;  // '1' para acordar, '0' para dormir
  int analogValue; // Valor analógico lido
} struct_message;

struct_message myData;

// Endereço MAC do Raspberry Pi (substitua pelo endereço correto)
uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0xF1, 0x74, 0x94}; // Adicionei o ponto e vírgula aqui

// Declaração da função de callback
void onDataReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len);

void setup() {
  // Inicializa a Serial
  Serial.begin(115200);

  // Inicializa o Wi-Fi no modo Station
  WiFi.mode(WIFI_STA);
  Serial.println("Wi-Fi iniciado");

  // Inicializa o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW iniciado");

  // Registra a função de callback para receber dados
  esp_now_register_recv_cb(onDataReceive);

  // Adiciona o Raspberry Pi como peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erro ao adicionar peer");
    return;
  }
}

// Definição da função de callback para quando os dados são recebidos
void onDataReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Received command: ");
  Serial.println(myData.command);

  // Ação com base no comando recebido
  if (myData.command == '1') {
    Serial.println("Acordar dispositivo");
    isAwake = true;
  } else if (myData.command == '0') {
    Serial.println("Dormir dispositivo");
    isAwake = false;
  }
}

void loop() {
  if (isAwake) {
    // Lê o valor analógico do pino GPIO 32
    int analogValue = analogRead(32);
    myData.analogValue = analogValue;

    // Envia o valor analógico para o Raspberry Pi
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    if (result == ESP_OK) {
      Serial.print("Valor analógico enviado: ");
      Serial.println(analogValue);
    } else {
      Serial.println("Erro ao enviar valor analógico");
    }
  }

  // Aguarda 1 segundo
  delay(1000);
}
~~~
#### Script do Pai (Raspberry)

~~~python

import bluetooth
import socket

# Endereço MAC do dispositivo receptor (substitua pelo endereço correto)
receiver_mac_address = 'C8:F0:9E:F1:74:94'

# Configuração do Bluetooth
def setup_bluetooth():
    server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    server_sock.bind(("", bluetooth.PORT_ANY))
    server_sock.listen(1)
    port = server_sock.getsockname()[1]
    bluetooth.advertise_service(server_sock, "RaspberryPiServer",
                                service_id="00001101-0000-1000-8000-00805F9B34FB",
                                service_classes=["00001101-0000-1000-8000-00805F9B34FB"])
    print(f"Bluetooth server started on port {port}")
    return server_sock

# Enviar comando via Wi-Fi
def send_command_via_wifi(command):
    # Substitua pelo IP e porta do dispositivo receptor
    receiver_ip = '192.168.1.100'
    receiver_port = 12345

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
        sock.sendto(command.encode(), (receiver_ip, receiver_port))
        print(f"Command '{command}' sent via Wi-Fi")

def main():
    server_sock = setup_bluetooth()
    client_sock, client_info = server_sock.accept()
    print(f"Accepted connection from {client_info}")

    try:
        while True:
            data = client_sock.recv(1024)
            if data:
                command = data.decode('utf-8').strip()
                if command in ['1', '0']:
                    print(f"Received command: {command}")
                    send_command_via_wifi(command)
    except OSError:
        pass
    finally:
        client_sock.close()
        server_sock.close()

if __name__ == "__main__":
    main()
~~~
