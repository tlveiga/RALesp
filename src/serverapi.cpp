#include "serverapi.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ServerAPI::ServerAPI(const ServerConfig &config)
{
  char esphostname[32];
  sprintf(esphostname, "RESP-%X\0", ESP.getChipId());
  WiFi.mode(WIFI_STA);
  WiFi.begin(config.ssid, config.password);
  WiFi.hostname(esphostname);
  Serial.print("Connecting to :");
  Serial.println(config.ssid);
  for (int i = 0; i < 150 && WiFi.status() != WL_CONNECTED; i++)
  {
    delay(100);
    //   drawLoadind(120, 8, i);
    //   display.display();
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("No connection. Changing to AP");
    WiFi.mode(WIFI_AP);
    IPAddress ip(90, 0, 0, 1);
    IPAddress nmask(255, 255, 255, 0);
    WiFi.softAPConfig(ip, ip, nmask);
    WiFi.softAP(esphostname);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
  }
  else
  {
    Serial.println("Connected to WiFi.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("success");
    //   printStatus(1, 1);
    //   display.fillRect(0, 16, 128, 8, BLACK);
    //   display.setCursor(0, 16);
    //   display.print("MQTT: ");
    if (strlen(config.mqtt_url) > 0)
    {
      // client.setServer(mqtt_server, 1883);
      // client.setCallback(mqtt_callback);
      // client.connect(mqtt_id);

      // display.print(mqtt_server);
      // display.display();

      // display.setCursor(120, 24);
      // for (int i = 0; i < 150 && !client.connected(); i++)
      // {
      //   delay(100);
      //   drawLoadind(120, 16, i);
      //   display.display();
      // }
    }
    else
    {
      // display.print("no conf");
    }
    //   printStatus(client.connected(), 2);
    //   display.display();
  }
}

ServerAPI::~ServerAPI()
{
}

void ServerAPI::loop()
{
}