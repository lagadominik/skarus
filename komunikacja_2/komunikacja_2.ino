#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#define PIN_LED      13 // Pin, do którego podpięty jest DIN
#define NUM_LEDS     6  // Twoje 6 diod

Adafruit_NeoPixel ring(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

typedef struct struct_message {
  int x;
  int y;
} struct_message;

struct_message incomingData;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  // Zamieniamy ruchy joysticka na kolory (0-255)
  int r = map(incomingData.x, 0, 4095, 0, 255);
  int g = map(incomingData.y, 0, 4095, 0, 255);
  int b = 150; // Stały niebieski, żeby zawsze coś świeciło

  // Ustawiamy kolor dla wszystkich 6 diod
  for(int i=0; i<NUM_LEDS; i++) {
    ring.setPixelColor(i, ring.Color(r, g, b));
  }
  
  ring.show(); // Wyślij dane do pierścienia (bez tego nie zaświeci!)
}

void setup() {
  Serial.begin(115200);
  
  ring.begin();           // Inicjalizacja pierścienia
  ring.setBrightness(50); // Ustaw jasność (0-255), żeby Cię nie oślepiło!
  ring.show();            // Wyłącz diody na starcie

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {}