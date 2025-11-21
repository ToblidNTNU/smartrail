#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define ssid "ESP32_gruppe7"
#define password "1234567"

#define redLedPin 2

IPAddress local_ip(192,168,1,1);    
IPAddress gateway(192,168,1,1);      
IPAddress subnet(255,255,255,0);     

WebServer server(80);

bool LedStatus = LOW;

void handle_OnConnect();
void handle_ledON();
void handle_ledOFF();
void handle_NotFound();

String getHTML() {
  String htmlcode = "<!DOCTYPE html><html>";
  htmlcode += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">";
  htmlcode += "<title>LED Control</title></head>";

  htmlcode += "<body>";
  htmlcode += "<h1>ESP32 Web Server</h1>";
  htmlcode += "<h3>A simple demo using access point mode</h3>";

  if (LedStatus) {
    htmlcode += "<p>Red LED Status: ON</p><a href=\"/ledoff\">Turn it OFF</a>";
  } else {
    htmlcode += "<p>Red LED Status: OFF</p><a href=\"/ledon\">Turn it ON</a>";
  }

  htmlcode += "</body></html>";

  return htmlcode;
}

void setup() {
  pinMode(redLedPin, OUTPUT);

  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(1000);

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledON);
  server.on("/ledoff", handle_ledOFF);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}

void loop() {
  server.handleClient();

  if (LedStatus) {
    digitalWrite(redLedPin, HIGH);
  } else {
    digitalWrite(redLedPin, LOW);
  }
}

void handle_OnConnect() {
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", getHTML());
}

void handle_ledON() {
  LedStatus = HIGH;
  Serial.println("LED Status: ON");
  server.send(200, "text/html", getHTML());
}

void handle_ledOFF() {
  LedStatus = LOW;
  Serial.println("LED Status: OFF");
  server.send(200, "text/html", getHTML());
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
