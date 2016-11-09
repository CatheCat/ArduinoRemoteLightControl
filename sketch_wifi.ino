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

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(VCC, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
