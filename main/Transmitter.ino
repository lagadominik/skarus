#include <esp_now.h>
#include <WiFi.h>

const int pinX  = 34;
const int pinY  = 35;
const int pinSW = 32;

// ⚠️ Wpisz MAC adres odbiornika!
uint8_t RECEIVER_MAC[] = {0x80, 0xF3, 0xDA, 0x55, 0x3E, 0x94};

const float V_MAX       = 0.5;    // m/s
const float OMEGA_MAX   = 3.0;    // rad/s

struct DrivePacket {
  float v;
  float omega;
};

void onSent(const wifi_tx_info_t* info, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  pinMode(pinSW, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, RECEIVER_MAC, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);
}

void loop() {
  int xRaw = analogRead(pinX);
  int yRaw = analogRead(pinY);

  // Mapowanie 0–4095 -> -1.0 do 1.0
  float xNorm = ((float)xRaw / 2047.5) - 1.0;   // oś obrotu
  float yNorm = ((float)yRaw / 2047.5) - 1.0;   // oś prędkości

  // Strefa martwa
  if (abs(xNorm) < 0.1) xNorm = 0.0;
  if (abs(yNorm) < 0.1) yNorm = 0.0;

  DrivePacket packet;
  packet.v     =  yNorm * V_MAX;
  packet.omega = -xNorm * OMEGA_MAX;

  esp_now_send(RECEIVER_MAC, (uint8_t*)&packet, sizeof(packet));

  Serial.printf("v: %.2f m/s | omega: %.2f rad/s\n", packet.v, packet.omega);
  delay(50);  // 20 Hz
}
