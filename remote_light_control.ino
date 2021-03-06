// 加入引用的標頭
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <WiFiUdp.h>

// 宣告Udp物件
WiFiUDP Udp;

// [全域變數]
// 連接WiFi SSID and passwd
const char* ssid = "AP NAME";
const char* password = "PASSWORD";
// 定義腳位
const int VCC = 16;
const int ledRGB[] = { 13, 12, 14 };
const int myLed[] = { 4, 5, 0, 2, 15 };
// UDP通訊port
unsigned int port = 13000;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
char replyBuffer[] = "read";
// Blink
byte isBlink = 0;
byte isOn = 0;
long previousTime;
// Delay Time
long myDelay = 1000;

void printWiFiStatus();

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);

  for (int i = 0; i < sizeof(ledRGB); i++) {
    pinMode(ledRGB[i], OUTPUT);
    digitalWrite(ledRGB[i], HIGH);
  }

  for (int i = 0; i < sizeof(myLed); i++) {
    pinMode(myLed[i], OUTPUT);
    digitalWrite(myLed[i], HIGH);
  }
  
  Serial.print("Wait for connect to WiFi.");
  if (WiFi.status() != WL_CONNECTED) {
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    printWiFiStatus();
  }
  Udp.begin(port);
}

void printWiFiStatus() {
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int readPacket() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet size: ");
    Serial.println(packetSize);
    Serial.print("From: ");
    IPAddress remote = Udp.remoteIP();

    for (int i = 0; i < 4; i++) {
      Serial.print(remote[i], DEC);

      if (i < 3) {
        Serial.print(".");
      }
    }

    Serial.print(", Port: ");
    Serial.println(Udp.remotePort());
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyBuffer);
    Udp.endPacket();
  }

  delay(10);
  return (packetSize);
}

void packageName() {
  // 讀取接收到的動作(字串)
  String str(packetBuffer);
  if (strcmp(packetBuffer, "ON") == 0) {
    PowerOn();
  } else if (strcmp(packetBuffer, "OFF") == 0) {
    PowerOff();
  } else if (strcmp(packetBuffer, "REDON") == 0) {
    RedOn();
  } else if (strcmp(packetBuffer, "REDOFF") == 0) {
    RedOff();
  } else if (strcmp(packetBuffer, "BLUEON") == 0) {
    BlueOn();
  } else if (strcmp(packetBuffer, "BLUEOFF") == 0) {
    BlueOff();
  } else if (strcmp(packetBuffer, "GREENON") == 0) {
    GreenOn();
  } else if (strcmp(packetBuffer, "GREENOFF") == 0) {
    GreenOff();
  } else if (strcmp(packetBuffer, "BLINKON") == 0) {
    isBlink = 1;
  } else if (strcmp(packetBuffer, "BLINKOFF") == 0) {
    isBlink = 0;
  }

  Serial.println(packetBuffer);
  memset(packetBuffer, 0, sizeof(packetBuffer));
}

void PowerOn() {
  digitalWrite(VCC, HIGH);
  LedAllOn();

  RedOn();
  BlueOn();
  GreenOn();
}

void PowerOff() {
  digitalWrite(VCC, LOW);

  LedAllOff();

  RedOff();
  BlueOff();
  GreenOff();
}

void LedOn(int ledId) {
  digitalWrite(myLed[ledId], LOW);
}

void LedAllOn() {
  for (int i = 0; i < sizeof(myLed); i++) {
    LedOn(i);
  }
}

void LedOff(int ledId) {
  digitalWrite(myLed[ledId], HIGH);
}

void LedAllOff() {
  for (int i = 0; i < sizeof(myLed); i++) {
    LedOff(i);
  }
}

void RedOn() {
  digitalWrite(ledRGB[0], LOW);
}

void RedOff() {
  digitalWrite(ledRGB[0], HIGH);
}

void BlueOn() {
  digitalWrite(ledRGB[1], LOW);
}

void BlueOff() {
  digitalWrite(ledRGB[1], HIGH);
}

void GreenOn() {
  digitalWrite(ledRGB[2], LOW);
}

void GreenOff() {
  digitalWrite(ledRGB[2], HIGH);
}

void FlashAll() {
  if (isBlink == 1) {
    unsigned long currentTime = millis();

    if (currentTime - previousTime > myDelay) {
      if (isOn == 0) {
        LedAllOn();
        isOn = 1;
      }
      else {
        LedAllOff();
        isOn = 0;
      }

      previousTime = currentTime;
    }
  }
}

void loop(void) {
  if (readPacket()) {
    Serial.println("Read.");
    packageName();
  }
  FlashAll();
}
