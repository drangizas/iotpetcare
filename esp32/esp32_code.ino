/*
 * IOT pet care
 * 2019
 * 
 */
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

// Konstantos
#define led_info 2
#define servo_pin 13
#define servo_spinTime 1050

// Kintamieji
int blinkMode;

// Servas
Servo serv;

// Wi-Fi duomenys
const char* ssid = "";
const char* pswd = "";

// Web prieiga
AsyncWebServer server(80);


// Setup'as --------------------------
void setup()
{
  Serial.begin(115200);

  // Apatinis ledas  
  pinMode(led_info, OUTPUT);
  blinkMode = HIGH;
  digitalWrite(led_info, blinkMode);
  
  // Servas
  serv.attach(servo_pin);

  // HTTP
  connectWifi();
  httpRoutes();

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
  blinkMode = LOW;
  Serial.println("");
  Serial.println("Prisijungta");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// Klausytis uzklausu
void httpRoutes()
{
  // Pagrindinės užklausos
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.print("Prisijungimas is IP: ");
    Serial.println(request->client()->remoteIP());
    
    if(!request->authenticate("", ""))
    {
      request->send(401, "text/plain", "Autorizacija nepavyko");
      info_led_blink();
      return;
    }
      
    // Ar yra reikiami parametrai    
    if (!request->hasParam("module"))
    {
      request->send(400, "text/plain", "Nera tinkamu parametru");
      info_led_blink();
      return;
    }
    
    AsyncWebParameter *p = request->getParam("module");
    String param = p->value().c_str();
    if (param.equals("spin"))
    {
      if (!request->hasParam("n"))
      {
        request->send(400, "text/plain", "Nenurodytas apsisukimu kiekis");
        info_led_blink();
        return;
      }
      
      p = request->getParam("n");
      int n = p->value().toInt();

      if (n < 1)
      {
        request->send(400, "text/plain", "Apsisukimu kiekis turi buti >= 1");
        info_led_blink();
        return;
      }
        
      String res = "Sekmingai pradeti vykdyti ";
      res += n;
      res += " apsukimai";
      request->send(200, "text/plain", res);
      info_led_blink();
      
      for (int i = 0; i < n; i++)
      {
        serv.write(180);
        delay(servo_spinTime);
        serv.write(90);
      }
      return;
    }

    request->send(404, "text/plain", "Neteisinga uzklausa...");
    info_led_blink();
  });

  // Nerasta užklausa
  server.onNotFound([](AsyncWebServerRequest *request)
  {
    request->send(404, "text/plain", "Neteisinga uzklausa");
    info_led_blink();
  });
}

void info_led_blink()
{
    blinkMode = HIGH;
    delay(100);
    blinkMode = LOW; 
}