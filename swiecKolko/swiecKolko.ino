#include <Adafruit_NeoPixel.h>

#define PIN            12   
#define NUMPIXELS      7   
#define MAX_BRIGHTNESS 255 // Maksymalna jasność chcem aby oslepial

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); 
}

void loop() {
  //wybierz se kolorek
  swiecKolko(3); 
}

void swiecKolko(int tryb) {
  int r, g, b;                      // Zmienne pomocnicze dla kolorów
  

  // SWITCH - wybór konkretnego koloru
  switch (tryb) {
    case 0:          // TRYB 0: Intensywny Czerwony
      r = 255; g = 0; b = 0;
      break;
    case 1:          // TRYB 1: Fioletowy / Magentowy
      r = 200; g = 0; b = 255;
      break;
    case 2:          // TRYB 2: Turkusowy
      r = 0; g = 255; b = 200;
      break;
    default:         // TRYB DOMYŚLNY: Biały
      r = 255; g = 255; b = 255;
      break;
  }

  
  for (int i = 0; i < NUMPIXELS; i++) {
   
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
  
}
