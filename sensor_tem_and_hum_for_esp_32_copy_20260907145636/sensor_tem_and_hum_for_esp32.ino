#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define dht1pin 4

const char* SSID = "Hong Huong";
const char* PASSWORD = "0963893224";
const char* SERVER_URL = "http://192.168.1.16:3000/data";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class dht11 {
private:
  char[4] ma_dht;
  uint8_t pin;
  uint8_t data[5];
  bool readData() {
    //gửi tín hiệu bắt đầu nhận dât.
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    //nhaan  data
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    pinMode(pin, INPUT_PULLUP);
    //check dc chua
    while (digitalRead(pin) == HIGH) {
      if (micros() - t > 81)
        return false;
    }
    //ddocj bit
    for (int i = 0; i < 5; i++) {
      data[i] = 0;  //set duwx lieeuj = 0;
      for(int j =0; J<8;j++)
      {
          unsigned long start = micros();
        while (digitalRead(pin) == HIGH) {
          if (micros() - start > 100)
            return false;
        }
        data[i] <<= 1; // dichj trais 1 bit ddeer them bit ms do ddc vaof
        unsigned long duration = micros() - start;
        if(duration > 40) data[i] |=1;
      }
    }
    return true;
  }
public:
  dht11(char[4] s = "", uint8_t pin = 0) {
    ma_dht = s;
    pin = dht11pin;
    data = null;
  }
    bool read() {

        if (!readData())         // Nếu đọc dữ liệu thô thất bại
            return false;           // Báo đọc thất bại

        // Tính checksum từ 4 byte dữ liệu đầu tiên
        uint8_t sum = data[0] + data[1]
                    + data[2] + data[3];
        return sum == data[4];     // So sánh checksum với byte thứ 5
    }
    // Trả về nhiệt độ phần nguyên
    int getTemperature() {
        return data[2];            // Byte thứ 3 chứa nhiệt độ phần nguyên
    }
    // Trả về độ ẩm phần nguyên
    int getHumidity() {
        return data[0];            // Byte thứ 1 chứa độ ẩm phần nguyên
    }
};

void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting WiFi");

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
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
  dht11 dht1(dht1,dht1pin);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED failed!");
    while (true)
      ;
  }
  display.clearDisplay();
  display.display();
  initWifi();
}

void loop() {

  float t1 = dht1.getTemperature();
  float h1 = dht1.getHumidity();

  if (dhtOK && WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"temp\":" + String(t1) + ",\"hum\":" + String(h1) + "}";

    int code = http.POST(json);

    Serial.print("Temp: ");
    Serial.print(t1);
    Serial.print(" | Hum: ");
    Serial.println(h1);

    Serial.print("HTTP Code: ");
    Serial.println(code);
    if (code <= 0) {
      Serial.print("HTTP Error: ");
      Serial.println(http.errorToString(code));
    }

    http.end();
  } else {
    Serial.print("Sensor/WiFi error. DHT: ");
    Serial.print(dhtOK ? "OK" : "FAIL");
    Serial.print(" WiFi: ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "OK" : "FAIL");
  }

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(28, 0);
  display.print("DHT11");

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

  display.setCursor(0, 52);

  if (dht1.read) {
    display.print("T:");
    display.print(t1);
    display.print("C H:");
    display.print(h1);
    display.print("%");
  } else {
    display.print("DHT ERROR");
  }

  display.display();

  delay(2000);
}