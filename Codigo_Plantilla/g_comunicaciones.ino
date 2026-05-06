void suscribirseATopics() {
  client.subscribe(topic_escucha);
}

void reconnect() {
  while (!client.connected()) {
    String client_id = "ESP32-Receptor-" + String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Conectado al Broker MQTT");
      suscribirseATopics();
    } else {
      Serial.print("Fallo conexión, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  Serial.print("Mensaje de RoboDK recibido: ");
  Serial.println(mensaje);

  if (mensaje == "S1_ON")  digitalWrite(LED_SENSOR1, LOW);
  if (mensaje == "S1_OFF") digitalWrite(LED_SENSOR1, HIGH);

  if (mensaje == "S2_ON")  digitalWrite(LED_SENSOR2, LOW);
  if (mensaje == "S2_OFF") digitalWrite(LED_SENSOR2, HIGH);

  if (mensaje == "READY")  digitalWrite(LED_READY, LOW);

  if (mensaje == "FULL")   digitalWrite(LED_PALET, LOW);
  if (mensaje == "EMPTY")  digitalWrite(LED_PALET, HIGH);
}