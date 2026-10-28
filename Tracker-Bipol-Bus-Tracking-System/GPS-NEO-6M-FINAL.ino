#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define RX 14
#define TX 12
#define GPS_BAUD 9600

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RX, TX);

const char* ssid = "hotspot1";
const char* password = "1234567899";
const char* mqtt_server = "broker.hivemq.com";

const char* mqtt_topic = "exploreCscKel3/bus";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublishTime = 0;
const long publishInterval = 5000;

float latitude;
float longitude;
String waktu = "";

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan ke WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("terhubung!");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  gpsSerial.begin(GPS_BAUD);
  Serial.println("Software Serial GPS dimulai");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Baca data GPS terus-menerus
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Cek apakah waktunya publish
  unsigned long currentMillis = millis();
  if (currentMillis - lastPublishTime >= publishInterval) {
    lastPublishTime = currentMillis;

    // Jika ada data baru dari GPS
    if (gps.location.isValid() && gps.time.isValid()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      waktu = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
    } else {
      // Default jika data tidak tersedia
      latitude = 0.0;
      longitude = 0.0;
      waktu = "00:00:00";
    }

    // Format JSON
    String jsonPayload = "{";
    jsonPayload += "\"latitude\": " + String(latitude, 6) + ",";
    jsonPayload += "\"longitude\": " + String(longitude, 6) + ",";
    jsonPayload += "\"waktu\": \"" + waktu + "\"";
    jsonPayload += "}";

    // Konversi ke buffer char*
    char buffer[256];
    jsonPayload.toCharArray(buffer, sizeof(buffer));

    // Publish
    client.publish(mqtt_topic, buffer);
    Serial.print("JSON dikirim ke topik [");
    Serial.print(mqtt_topic);
    Serial.print("]: ");
    Serial.println(jsonPayload);
  }
}