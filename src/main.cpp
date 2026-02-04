#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>
#include <NTPClient.h>
#include <Stepper.h>
#include <WiFiUdp.h>

#include "credentials.h"


const long utcOffsetInSeconds = 3600; // décalage horaire
const int dropHour = 19;
const int dropMinute = 0;
const int dropSecond = 0;
const int foodSlots = 16; // 16 emplacements de miamiam à l'allumage
const int numled = 20;
const int steps = 2048;
int brightness = 50;
int ledHues[numled];
int foodRemaining = foodSlots;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
Stepper moteur(steps, 14, 5, 13, 4);
CRGB leds[numled];

int bright360to255(int val) { return static_cast<int>((val / 360.0) * 255); }

void ledsShow(int mode) // mode off and aqua (0, 3)
{
  switch (mode) {
  case 0:
    for (int i = 0; i <= numled - 1; i++) {
      ledHues[i] = bright360to255(0); // Enregistrer la teinte
      leds[i].setHSV(ledHues[i], 0, 0);
      FastLED.show();
    }
    break;
  case 3:                                 // aqua mode
    for (int i = 0; i <= numled - 1; i++) // default color for aquarium
    {
      int tempHue = random(5, 10) * (random(0, 2) * 2 - 1);
      tempHue = ledHues[i] + (tempHue);
      if (tempHue > 156) // hue regulator
      {
        tempHue = 140;
      } else if (tempHue < 106) {
        tempHue = 120;
      }
      leds[i].setHSV(tempHue, 255, brightness);
      ledHues[i] = tempHue; // Mettre à jour la teinte enregistrée
      Serial.print(tempHue);
      Serial.print(" - ");
      FastLED.show();
      delay(1000 / numled);
    }
    break;

  default:
    break;
  }
  yield();
}

void ledsShow(int mode, int h, int b) // mode normal and blinking (1 , 2)
{
  switch (mode) {
  case 1: // normal
    for (int i = 0; i <= numled - 1; i++) {
      ledHues[i] = bright360to255(h); // Enregistrer la teinte
      leds[i].setHSV(ledHues[i], 255, b);
      FastLED.show();
    }
    break;

  case 2: // blinking
    ledsShow(1, h, b);
    delay(500);
    ledsShow(0);
    delay(500);
    break;

  default:
    break;
  }
  yield();
}

void wifiNetworkResearcher() { // module wifi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("\nConnexion en cours");
    int timeSpend = 0;
    for (int i = 0; i < 3; i++) {
      WiFi.begin(wifi[i][0], wifi[i][1]);
      while (WiFi.status() != WL_CONNECTED || timeSpend == 10) {
        ledsShow(2, 245, 50);
        Serial.print(".");
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 12>(leds, numled);
  WiFi.begin(ssid, password);
  Serial.print("\nConnexion");
  while (WiFi.status() != WL_CONNECTED) {
    ledsShow(2, 245, 50);
    Serial.print(".");
  }
  Serial.print("\n");
  timeClient.begin();
  moteur.setSpeed(5); // speed
  Serial.println("Distributeur démarré et connecté");
  Serial.print("Heure de distribution du repas: ");
  Serial.print(dropHour);
  Serial.print(":");
  Serial.print(dropMinute);
  Serial.print(":");
  Serial.println(dropSecond);
  Serial.print("Heure actuelle: ");
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  ledsShow(1, 145, 50);
  delay(1000);
  ledsShow(1, 180, brightness);
}

void loop() {
  timeClient.update();
  // set brightness
  if (timeClient.getHours() < 22 && timeClient.getHours() >= 7) {
    brightness = 75; // jour
  } else {
    brightness = 25; // nuit
  }
  if (foodRemaining > 0) {
    ledsShow(3);
  } else {
    ledsShow(2, 0, brightness);
  }
  if (timeClient.getHours() == dropHour &&
      timeClient.getMinutes() == dropMinute &&
      timeClient.getSeconds() == dropSecond)
  // if (timeClient.getSeconds() % 5 == 0) // only for tests
  {
    moteur.step(steps / foodSlots);
    if (foodRemaining > 0) {
      foodRemaining--;
    }
    Serial.println("C'est le moment de manger");
    Serial.print("Quantité restante: ");
    Serial.println(foodRemaining);
  }
  Serial.print(brightness);
  Serial.print(" - ");
  Serial.println(timeClient.getFormattedTime());
  yield();
}