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

void loop() {
  // put your main code here, to run repeatedly:

}
