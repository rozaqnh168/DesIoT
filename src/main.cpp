#include <Arduino.h>

#define BLYNK_TEMPLATE_ID "ISI_TEMPLATE_ID_ANDA_DISINI"
#define BLYNK_TEMPLATE_NAME "ISI_NAMA_TEMPLATE_ANDA_DISINI"
#define BLYNK_AUTH_TOKEN "ISI_AUTH_TOKEN_ANDA_DISINI"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ThingSpeak.h>
#include <DHT.h>

char ssid[] = "NAMA_WIFI_ANDA";
char pass[] = "PASSWORD_WIFI_ANDA";

unsigned long myChannelNumber = 1234567; // Ganti dengan angka Channel ID
const char * myWriteAPIKey = "ISI_WRITE_API_KEY_ANDA";

#define DHTPIN 4
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
BlynkTimer timer;

void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Jika gagal baca, langsung keluar fungsi tanpa print pesan error
  if (isnan(h) || isnan(t)) {
    return;
  }

  // Tampilkan data yang berhasil dibaca di Serial Monitor
  Serial.print("Suhu: "); Serial.print(t); Serial.println(" *C");
  Serial.print("Kelembapan: "); Serial.print(h); Serial.println(" %");

  // Kirim data ke Blynk
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  // Kirim data ke ThingSpeak (tanpa print status HTTP/error)
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  ThingSpeak.begin(client);

  // Interval pengiriman data setiap 20 detik
  timer.setInterval(20000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}