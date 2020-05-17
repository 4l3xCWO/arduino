 // Pisca led da placa e mais 5 leds, portas D1, D2, D5, D6, D7
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS 1000
 
// Logar na rede
const char *ssid = "Marcelo_OI_2G";
const char *password = "sdkkds00";

// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;

float BPM, SpO2;
uint32_t tsLastReport = 0;
 
ESP8266WebServer server(80); //instanciar servidor no porto 80
 
String page = "";
String titulo = "";
String led1 = "";
String led2 = "";
String led3 = "";
String leds = "";
String batimento = "";
String o2 = "";

void onBeatDetected()
{
    Serial.println("Beat Detected!");
     
  /*     digitalWrite(LED_BUILTIN, LOW);
     delay(50);
     digitalWrite(LED_BUILTIN, HIGH);
     delay(50);
     digitalWrite(LED_BUILTIN, LOW);
     delay(50);
     digitalWrite(LED_BUILTIN, HIGH);
     delay(50);
     */
}    
void setup(void)
{

pinMode(16, OUTPUT); //oximetro
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);
pinMode(14, OUTPUT);
pinMode(LED_BUILTIN, OUTPUT);

  //Pagina HTML
  titulo = "<h1>Conexao NodeMCU Web Server</h1></p>";
  led1 = "<h1>Led 1  </h1><a href=\"LED1On\"><button>ON</button></a>&nbsp;<a href=\"LED1Off\"><button>OFF</button></a>";
  led2 = "<h1>Led 2  </h1><p><a href=\"LED2On\"><button>ON</button></a>&nbsp;<a href=\"LED2Off\"><button>OFF</button></a>";
  led3 = "<h1>Led 3  </h1><p><a href=\"LED3On\"><button>ON</button></a>&nbsp;<a href=\"LED3Off\"><button>OFF</button></a>";
  leds = "<h1>Todos os LEDS</h1><p><a href=\"LEDsOn\"><button>ON</button></a>&nbsp;<a href=\"LEDsOff\"><button>OFF</button></a></p>"; 
  batimento = "<h1>Batimento cardiaco (BPM): </h1>";
  o2 = "<h1>%O2: </h1>";
  
 

//teste de leds
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(12, HIGH); 
  delay(100);
  digitalWrite(13, HIGH); 
  delay(100);
  digitalWrite(14, HIGH); 
  delay(100);

  Serial.begin(9600);
  WiFi.begin(ssid, password); //Iniciar ligação à rede Wi-Fi
  Serial.println("");
 
  // Aguardar por ligação
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Ligado a: ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

 digitalWrite(LED_BUILTIN, HIGH);
   delay(100);
  digitalWrite(12, LOW); 
   delay(100);
   

  if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
        
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 digitalWrite(13, LOW); 
   delay(100);
  digitalWrite(14, LOW); 
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  server.on("/", []() {
    server.send(200, "text/html", page);
  });
  server.on("/LEDOn", []() {
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  });
  server.on("/LEDOff", []() {
    server.send(200, "text/html", page);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  });
  
  server.on("/LED1On", []() {
    server.send(200, "text/html", page);
    digitalWrite(12, HIGH);
    delay(100);
  });
  server.on("/LED1Off", []() {
    server.send(200, "text/html", page);
    digitalWrite(12, LOW);
    delay(100);
  });

  server.on("/LED2On", []() {
    server.send(200, "text/html", page);
    digitalWrite(13, HIGH);
    delay(100);
  });
  server.on("/LED2Off", []() {
    server.send(200, "text/html", page);
    digitalWrite(13, LOW);
    delay(100);
  });
 
  server.on("/LED3On", []() {
    server.send(200, "text/html", page);
    digitalWrite(14, HIGH);
    delay(100);
  });
  server.on("/LED3Off", []() {
    server.send(200, "text/html", page);
    digitalWrite(14, LOW);
    delay(100);
  });
  
 server.on("/LEDsOn", []() {
    server.send(200, "text/html", page);
    
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(14, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  });
  server.on("/LEDsOff", []() {
    server.send(200, "text/html", page);
   
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    digitalWrite(14, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  });
  
  server.begin();
  Serial.println("Web server Inicializado!");
}
 
void loop(void)
{

  pox.update();
    
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
        
      page = titulo + batimento + (BPM) + o2 + (SpO2) + led1 + led2 + led3 + leds;

      
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
        tsLastReport = millis();
    }
  
  server.handleClient();
  

  
}
