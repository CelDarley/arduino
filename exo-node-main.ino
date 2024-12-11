#include <esp_now.h>  
#include <WiFi.h>  

// Endereço MAC do segundo ESP32 (substitua pelo endereço correto)  
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};  

// Estrutura da mensagem  
typedef struct struct_message {  
  String message;  
} struct_message;  

struct_message myData;  

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

  // Registra o endereço do receptor  
  esp_now_peer_info_t peerInfo;  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);  
  peerInfo.channel = 0;    
  peerInfo.encrypt = false;  

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {  
    Serial.println("Erro ao adicionar peer");  
    return;  
  }  
}  

void loop() {  
  // Envia uma mensagem para o segundo ESP32  
  myData.message = "Acorde!";  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));  

  if (result == ESP_OK) {  
    Serial.println("Mensagem enviada com sucesso");  
  } else {  
    Serial.println("Erro ao enviar mensagem");  
  }  

  delay(5000); // Aguarda 5 segundos antes de enviar novamente  
}  
