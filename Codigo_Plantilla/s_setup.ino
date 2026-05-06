void on_setup() {
  Serial.begin(115200);

  pinMode(LED_SENSOR1, OUTPUT);
  pinMode(LED_SENSOR2, OUTPUT);
  pinMode(LED_READY, OUTPUT);
  pinMode(LED_PALET, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado");

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}