#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <math.h>

// Ganti sesuai dengan WiFi & MQTT kamu
const char* ssid = "Redmi 13C";
const char* password = "123457890";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "exploreCscKel3/bus";

// LCD I2C (0x27 atau 0x3F tergantung modul)
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient espClient;
PubSubClient client(espClient);

// Struktur data halte
struct Halte {
  const char* nama;
  float lat;
  float lon;
};

// Daftar halte (ganti sesuai lokasi sebenarnya)
Halte daftarHalte[] = {
  { "Titik Awal Bipol", -6.3711018, 106.8237532 },
  { "Halte Vokasi", -6.366127, 106.821670 },
  { "Halte SOR", -6.366812, 106.823784 },
  { "Halte FMIPA", -6.3698232,  106.8258449 },
  { "Parkiran", -6.371597, 106.824914 },
};

// Radius toleransi (0.0005 derajat ≈ ±50 meter)
const float radiusToleransi = 0.0005

;

// Fungsi untuk deteksi halte
const char* deteksiHalte(float lat, float lon) {
  for (int i = 0; i < sizeof(daftarHalte) / sizeof(daftarHalte[0]); i++) {
    float deltaLat = lat - daftarHalte[i].lat;
    float deltaLon = lon - daftarHalte[i].lon;
    float jarak = sqrt(deltaLat * deltaLat + deltaLon * deltaLon);
    if (jarak <= radiusToleransi) {
      return daftarHalte[i].nama;
    }
  }
  return "Di Perjalanan";
}

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
  Serial.println("\nWiFi Terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String jsonStr = "";
  for (unsigned int i = 0; i < length; i++) {
    jsonStr += (char)payload[i];
  }

  Serial.print("Pesan diterima: ");
  Serial.println(jsonStr);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);

  if (error) {
    Serial.print("Gagal parsing JSON: ");
    Serial.println(error.f_str());
    return;
  }

  float lat = doc["latitude"];
  float lon = doc["longitude"];
  Serial.print("Latitude: ");
  Serial.println(lat, 6);
  Serial.print("Longitude: ");
  Serial.println(lon, 6);

  const char* namaHalte = deteksiHalte(lat, lon);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lokasi Bus:");
  lcd.setCursor(0, 1);
  lcd.print(namaHalte);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ESP8266DisplayClient")) {
      Serial.println("Terhubung!");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Menunggu data...");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}