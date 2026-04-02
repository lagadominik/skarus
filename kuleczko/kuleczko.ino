#include <Adafruit_NeoPixel.h>

#define PIN        12  // Pin, do którego podłączone jest Data IN
#define NUMPIXELS  7  // Liczba diod w Jewel (standardowo 7)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin(); // Inicjalizacja biblioteki
}

void loop() {
  pixels.clear(); // Wyłączenie wszystkich diod

 
    pixels.setPixelColor(6, pixels.Color(63, 112, 235));
    pixels.show();   // Wysłanie danych do diod
    delay(100);
  
}
