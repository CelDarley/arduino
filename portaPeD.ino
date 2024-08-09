#include <ESP8266WiFi.h>
#include <SPI.h>


const char* ssid = "Roboflex - ADM 2G"; //Enter SSID
const char* password = "Roboflex16@2022"; //Enter Password

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);      
    delay(10);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    Serial.println(password);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());   
    server.begin();

}
  
void loop(){
 WiFiClient client = server.available();  

  if (client) {                           
    Serial.println("New Client.");        
    String currentLine = "";              
    while (client.connected()) {          
      if (client.available()) {           
        char c = client.read();           
        Serial.write(c);                  
        if (c == '\n') {                  

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<meta charset='utf-8'>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no>");
            
            client.print("<div style='display:flex;flex-flow:column;justify-content:center;width:100%;margin-top:40px'>");
            
              client.print("<div style='background-color:black;border-roud:3px;width:90%;margin-bottom:50px;margin-right:5%;margin-left:5%;'><img src='https://roboflex.com.br/wp-content/uploads/2023/05/logotipo-roboflex.png'/></div>");
                  
              client.print("<div style='margin-right:5%;margin-left:5%;width:90%;background-color: rgba(31, 104, 222, 0.13);'>");
                client.print("<div style='border-radius:3px;padding-top:30px;padding-bottom:20px'>");
                  client.print("<div style='display:flex;justify-content:center;font-size:18px;font-family:fantasy;margin-top:30px'>Clique para abrir porta</div>");
                  client.print("<div style='display:flex;justify-content:center;font-size:20px;margin-top:20px;font-family: sans-serif;margin-bottom:50px;'><a href=\"/H\" class='card-link'>Porta P&D</a></div>");
               /*   client.print("<div style='display:flex;flex-flow:row;justify-content:center;'>");
                  client.print("<div style='display:flex;justify-content:center;margin-top:20px;margin-right:0;border: 2px solid #3571CD;border-radius: 1px;width:10px;height:40px;'></div>");
                  client.print("<div style='display:flex;justify-content:center;margin-top:20px;margin-left:0;border: 2px solid #3571CD;border-radius: 1px;width:10px;height:40px;animation: 3s 1 alternate slidein;@keyframes slidein{from {margin-left: 100%;width: 300%;}to{margin-left: 0%;width: 100%;}}'></div>");
                  client.print("</div>");*/
                client.print("</div>");
             
              client.print("</div>");
       
            client.print("</div>");
            client.println();
            
            break;
          } else {  
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }

        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);
          delay(2000);
          digitalWrite(5, LOW);
        }
        
      }
    }
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
