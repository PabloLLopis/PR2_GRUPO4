#include <WiFi.h>
#include <PubSubClient.h>

// 1. Configuración de Red y Broker (Mantén los de tu laboratorio)
const char *ssid = "UPV-PSK";
const char *password = "giirob-pr2-2023";
const char *mqtt_broker = "mqtt.dsic.upv.es";
const char *mqtt_username = "giirob";
const char *mqtt_password = "UPV2024";
const int mqtt_port = 1883;

// 2. El canal (topic) donde vas a ESCUCHAR a RoboDK
// IMPORTANTE: Este debe ser el mismo que pongas en el script de Python
const char *topic_escucha = "giirob/pr2/estacion/leds";

// 3. Definición de tus 4 LEDs
const int LED_SENSOR1 = 13;
const int LED_SENSOR2 = 12;
const int LED_READY   = 14;
const int LED_PALET   = 15;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
 
  pinMode(LED_SENSOR1, OUTPUT);
  pinMode(LED_SENSOR2, OUTPUT);
  pinMode(LED_READY, OUTPUT);
  pinMode(LED_PALET, OUTPUT);

  conectarWiFi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback); // Aquí es donde recibes los mensajes
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado");
}

void reconnect() {
  while (!client.connected()) {
    String client_id = "ESP32-Receptor-" + String(WiFi.macAddress());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Conectado al Broker MQTT");
      client.subscribe(topic_escucha); // Te suscribes para recibir órdenes
    } else {
      Serial.print("Fallo conexión, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

// ESTA FUNCIÓN SE EJECUTA CUANDO ROBODK ENVÍA UN MENSAJE
void callback(char *topic, byte *payload, unsigned int length) {
  String mensaje = "";
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  Serial.print("Mensaje de RoboDK recibido: ");
  Serial.println(mensaje);

  // LÓGICA DE TUS 4 LEDS
  if (mensaje == "S1_ON")  digitalWrite(LED_SENSOR1, LOW);
  if (mensaje == "S1_OFF") digitalWrite(LED_SENSOR1, HIGH);

  if (mensaje == "S2_ON")  digitalWrite(LED_SENSOR2, LOW);
  if (mensaje == "S2_OFF") digitalWrite(LED_SENSOR2, HIGH);

  if (mensaje == "READY")  digitalWrite(LED_READY, LOW);
 
  if (mensaje == "FULL")   digitalWrite(LED_PALET, LOW);
  if (mensaje == "EMPTY")  digitalWrite(LED_PALET, HIGH);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
