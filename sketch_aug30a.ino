#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>

//WIFI
const char* SSID = "Hong Huong";
const char* PASSWORD = "0963893224";

//DHT
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//WIFI INIT
void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting WiFi");
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED &&
         millis() - startAttemptTime < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi FAILED (no crash)");
  }
}
void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED failed!");
    while (true);
  }

  display.clearDisplay();
  display.display();

  dht.begin();
  initWifi();
}
void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  bool dhtOK = !(isnan(t) || isnan(h));
  display.clearDisplay();

  // TITLE
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(28, 0);
  display.print("DHT11");

  // WIFI INFO
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("WiFi: ");
  display.print(WiFi.status() == WL_CONNECTED ? "OK" : "FAIL");

  display.setCursor(0, 30);
  display.print("IP:");
  display.print(WiFi.localIP());

  display.setCursor(0, 40);
  display.print("RSSI:");
  display.print(WiFi.RSSI());
  display.print(" dBm");

  // TEMP + HUMIDITY
  display.setCursor(0, 52);

  if (dhtOK) {
    display.print("T:");
    display.print(t);
    display.print("C H:");
    display.print(h);
    display.print("%");
  } else {
    display.print("DHT ERROR");
  }
  display.display();
  delay(2000);
}