//hleooooaoaooaoao
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>

// ================= WIFI =================
const char* SSID = "Uy Tin Viet";
const char* PASSWORD = "hiephanh";

// ================= DHT11 =================
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ================= WIFI INIT =================
void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  Serial.print("Connecting WiFi");

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi Failed!");
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  initWifi();

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED FAILED");
    while (1);
  }

  display.clearDisplay();
  display.display();
}

// ================= LOOP =================
void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("DHT11 read failed!");
    delay(2000);
    return;
  }

  // ================= SERIAL =================
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C | Hum: ");
  Serial.print(h);
  Serial.print(" % | RSSI: ");
  Serial.println(WiFi.RSSI());

  // ================= OLED =================
  display.clearDisplay();

  // TITLE
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(28, 0);
  display.print("DHT11");

  // WIFI INFO
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("IP:");
  display.println(WiFi.localIP());

  display.setCursor(0, 30);
  display.print("RSSI:");
  display.print(WiFi.RSSI());
  display.println(" dBm");

  // TEMP
  display.setCursor(0, 42);
  display.print("Temp: ");
  display.print(t);
  display.println(" C");

  // HUMIDITY
  display.setCursor(0, 54);
  display.print("Hum: ");
  display.print(h);
  display.println(" %");

  display.display();

  delay(2000);
}