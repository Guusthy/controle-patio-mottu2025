/////////--------IOT--------FIAP------------///////////

#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";  
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "mottu_mqtt";
const char *TOPIC_SUBSCRIBE_LED = "fiap/iot/led";
const char *TOPIC_PUBLISH_DISTANCIA = "mottu/patio";

#define TRIG_PIN_MAIN 13
#define ECHO_PIN_MAIN 12
#define TRIG_PIN_TESTE 14
#define ECHO_PIN_TESTE 27
#define PIN_LED_VERDE 15
#define PIN_LED_VERMELHO 2
#define PUBLISH_DELAY 1000
const int TAMANHO = 200;

WiFiClient espClient;
PubSubClient MQTT(espClient);
unsigned long publishUpdate = 0;

void initWiFi();
void initMQTT();
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();
float medirDistancia(int trigPin, int echoPin);

void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length) {
  String msg = String((char*)payload).substring(0, length);
  Serial.printf("Mensagem recebida via MQTT: %s do tópico: %s\n", msg.c_str(), topic);
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar com o Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao broker MQTT!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
    } else {
      Serial.println("Falha na conexão com MQTT. Tentando novamente em 2 segundos.");
      delay(2000);
    }
  }
}

void checkWiFIAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Wifi conectado com sucesso: ");
  Serial.println(SSID);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

float medirDistancia(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);
  float distancia = duracao * 0.034 / 2;

  return distancia;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN_MAIN, OUTPUT);
  pinMode(ECHO_PIN_MAIN, INPUT);
  pinMode(TRIG_PIN_TESTE, OUTPUT);
  pinMode(ECHO_PIN_TESTE, INPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_VERMELHO, OUTPUT);

  digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_VERMELHO, LOW);

  initWiFi();
  initMQTT();
}

void loop() {
  checkWiFIAndMQTT();
  MQTT.loop();

  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();

    float distanciaPrincipal = medirDistancia(TRIG_PIN_MAIN, ECHO_PIN_MAIN);
    float distanciaTeste = medirDistancia(TRIG_PIN_TESTE, ECHO_PIN_TESTE);

    if (distanciaPrincipal > 0 && distanciaPrincipal <= 50) {
      digitalWrite(PIN_LED_VERDE, HIGH);
      digitalWrite(PIN_LED_VERMELHO, LOW);
    } else {
      digitalWrite(PIN_LED_VERDE, LOW);
      digitalWrite(PIN_LED_VERMELHO, HIGH);
    }

    StaticJsonDocument<TAMANHO> doc;
    doc["sensor_principal"] = distanciaPrincipal;
    doc["sensor_teste"] = distanciaTeste;

    String json;
serializeJson(doc, json);

MQTT.publish(TOPIC_PUBLISH_DISTANCIA, json.c_str());
Serial.println(json);

  }
}
