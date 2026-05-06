#include <WiFi.h>
#include <PubSubClient.h>
#include "Config.h"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(BAUDS);

  WiFi.begin(NET_SSID, NET_PASSWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado");

  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);

  on_setup();  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  on_loop();  
}