#include <Adafruit_NeoPixel.h> // Dołączenie biblioteki do obsługi diod WS2812B

#define PIN            12      // Definicja pinu cyfrowego, do którego podpięty jest przewód DIN
#define NUMPIXELS      7       // Określenie liczby diod w Twoim pasku/kółku
#define BRIGHTNESS     255     // Ustawienie maksymalnej jasności (zakres 0-255)

// Inicjalizacja obiektu "strip", który reprezentuje naszą taśmę w kodzie
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();               // Uruchomienie komunikacji z diodami
  strip.setBrightness(BRIGHTNESS); // Nałożenie zdefiniowanego limitu jasności
  strip.show();                // Wysłanie danych do diod (teraz: wyczyść/wyłącz wszystkie)
}

void loop() {
  static float offset = 0;     // Zmienna "czasu" - statyczna, więc nie zeruje się przy nowym cyklu loop

  // --- OBLICZANIE KOLORÓW ---
  // sin(offset) zwraca wartość od -1 do 1. Dodajemy 1, żeby mieć 0 do 2. 
  // Mnożymy przez 127, żeby uzyskać zakres bajtowy (0-254) dla standardu RGB.

  int r = (sin(offset) + 1) * 127;         // Obliczanie natężenia koloru czerwonego
  int g = (sin(offset + 2.0) + 1) * 127;   // Zielony przesunięty o 2.0 (żeby nie startował razem z czerwonym)
  int b = (sin(offset + 4.0) + 1) * 127;   // Niebieski przesunięty o 4.0 (dla efektu tęczy)

  // --- PRZYPISANIE KOLORU DO DIOD ---
  for (int i = 0; i < NUMPIXELS; i++) {    // Pętla przechodząca po każdej z 7 diod
    strip.setPixelColor(i, strip.Color(r, g, b)); // Ustawienie tego samego koloru RGB dla diody nr "i"
  }

  // --- AKTUALIZACJA FIZYCZNA ---
  strip.show();                // "Wypchnięcie" ustawionych kolorów z pamięci Arduino do fizycznych diod
  
  // --- KONTROLA ANIMACJI ---
  offset += 0.03;              // Zwiększenie przesunięcia (im mniejszy krok, tym płynniejsza i wolniejsza zmiana)
  
  delay(20);                   // Krótka pauza (20 milisekund), aby animacja nie "migała" zbyt szybko
}