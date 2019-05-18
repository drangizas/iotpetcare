/*
 * IOT pet care
 * 2019
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>

// Informacinis led'as
#define led_info 2
#define servo_pin 13

int blinkMode, servPos;
int incomingByte = 0;   // for incoming serial data

// Servas
Servo serv;

// Wi-Fi duomenys
const char* ssid = "OK";
const char* pswd = "10eurmen";

// Web prieiga
AsyncWebServer server(80);


// Setup'as --------------------------
void setup()
{
  Serial.begin(115200);

  // Apatinis ledas  
  pinMode(led_info, OUTPUT);
  blinkMode = HIGH;

  // ESP Failų sistema
  if(!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Servas
  serv.attach(servo_pin);

  // HTTP užklausos
  connectWifi();
  listenForClients();

  server.begin();
}

// Ciklas ----------------------------
void loop()
{
  digitalWrite(led_info, blinkMode);
}

// Prisijungimas prie interneto
void connectWifi()
{
  Serial.print("Jungiamasi prie: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  //  Jei pavyko
  Serial.println("");
  Serial.println("Prisijungta");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// Klausytis uzklausu
void listenForClients()
{
  // Index
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    blinkMode = HIGH;
    request->send(SPIFFS, "/index.htm", "text/html");
    delay(100);
    blinkMode = LOW;
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    blinkMode = HIGH;
    request->send(SPIFFS, "/index.htm", "text/html");
    delay(100);
    blinkMode = LOW;
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    blinkMode = HIGH;
    request->send(SPIFFS, "/index.htm", "text/html");
    delay(100);
    blinkMode = LOW;
  });
}
