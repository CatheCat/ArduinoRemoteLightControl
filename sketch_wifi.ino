#include <ESP8266WiFi.h>
#include <SPI.h>
#include <WiFiUdp.h>

WiFiUDP Udp;
const char* ssid = "AP NAME";
const char* password = "PASSWORD";
const int VCC = 12;
const int ledRed = 13;
const int ledBlue = 14;
const int ledGreen = 16;
unsigned int port = 13000;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
char replyBuffer[] = "read";

void printWiFiStatus();

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(VCC, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  
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

void loop(void) {
  if (readPacket()) {
  
  }
}
