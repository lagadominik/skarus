#include <esp_now.h>
#include <WiFi.h>

// ==========================================
// TUTAJ WPISZ ADRES MAC DRUGIEJ PŁYTKI
// Format: {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}
// ==========================================
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Struktura wiadomości. MUSI być identyczna na obu płytkach!
typedef struct struct_message {
  int id;          // Identyfikator płytki (np. 1 dla pierwszej, 2 dla drugiej)
  int counter;     // Licznik wysłanych wiadomości
  float temp;      // Przykładowa wartość zmiennoprzecinkowa
  char text[32];   // Przykładowy tekst
} struct_message;

// Utworzenie zmiennych dla danych wysyłanych i odbieranych
struct_message myData;
struct_message incomingData;

esp_now_peer_info_t peerInfo;
int msgCount = 0;

// Callback: Wywoływany po wysłaniu danych (potwierdzenie dostarczenia)
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus wysyłania: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Dostarczono pomyślnie" : "BŁĄD dostarczenia");
}

// Callback: Wywoływany po odebraniu danych
void OnDataRecv(const esp_now_recv_info_t *esp_now_info, const uint8_t *incomingDataPtr, int len) {
  // Kopiowanie odebranych bajtów do naszej struktury
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  
  Serial.print("\n=== ODEBRANO WIADOMOŚĆ ===");
  Serial.print("\nOd MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", esp_now_info->src_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.printf("\nRozmiar: %d bajtów\n", len);
  Serial.printf("ID nadawcy: %d\n", incomingData.id);
  Serial.printf("Licznik: %d\n", incomingData.counter);
  Serial.printf("Temperatura: %.2f\n", incomingData.temp);
  Serial.printf("Tekst: %s\n", incomingData.text);
  Serial.println("==========================");
}

void setup() {
  Serial.begin(115200);

  // Ustawienie Wi-Fi w tryb stacji (Station)
  WiFi.mode(WIFI_STA);

  // Inicjalizacja ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Błąd podczas inicjalizacji ESP-NOW");
    return;
  }

  // Rejestracja funkcji callback dla wysyłania i odbierania
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Rejestracja "rówieśnika" (drugiej płytki)
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  // Kanał Wi-Fi (0 = domyślny)
  peerInfo.encrypt = false; // Brak szyfrowania dla uproszczenia
  
  // Dodanie drugiej płytki do listy
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Nie udało się dodać rówieśnika (peer)");
    return;
  }
}

void loop() {
  // Przygotowanie danych do wysłania
  // Wartości ID zmień na 1 dla pierwszej płytki i 2 dla drugiej, aby łatwo je rozróżnić
  myData.id = 1; 
  myData.counter = msgCount++;
  myData.temp = 22.5 + (random(0, 100) / 10.0); // Losowa "temperatura" do testów
  strcpy(myData.text, "Wiadomosc testowa!");

  // Wysłanie danych
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Wysłano dane z sukcesem");
  } else {
    Serial.println("Błąd wysyłania danych");
  }

  // Czekaj 5 sekund przed wysłaniem kolejnej wiadomości
  delay(5000);
}