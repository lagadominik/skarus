#include <esp_now.h>
#include <WiFi.h>

// Silnik prawy
#define R_PWM_1 25
#define L_PWM_1 26
// Silnik lewy
#define R_PWM_2 27
#define L_PWM_2 14

const float WHEEL_BASE   = 0.217;
const float WHEEL_RADIUS = 0.055;
const float V_MAX        = 0.5;

struct DrivePacket {
  float v;
  float omega;
};

void rotateMotor(const char* motor, int pwm, bool forward) {
  pwm = constrain(pwm, 0, 255);

  if (strcmp(motor, "right") == 0) {
    if (forward) {
      digitalWrite(L_PWM_1, LOW);
      analogWrite(R_PWM_1, pwm);
    } else {
      digitalWrite(R_PWM_1, LOW);
      analogWrite(L_PWM_1, pwm);
    }
  } else if (strcmp(motor, "left") == 0) {
    if (forward) {
      digitalWrite(L_PWM_2, LOW);
      analogWrite(R_PWM_2, pwm);
    } else {
      digitalWrite(R_PWM_2, LOW);
      analogWrite(L_PWM_2, pwm);
    }
  }
}

void drive(float v, float omega) {
  float v_right = v + (omega * WHEEL_BASE) / 2.0;
  float v_left  = v - (omega * WHEEL_BASE) / 2.0;

  int pwm_right = constrain((int)(abs(v_right) / V_MAX * 255.0), 0, 255);
  int pwm_left  = constrain((int)(abs(v_left)  / V_MAX * 255.0), 0, 255);

  Serial.println("------------------------------");
  Serial.printf("  v: %+.3f m/s  |  omega: %+.3f rad/s\n", v, omega);
  Serial.printf("  v_right: %+.3f m/s  ->  PWM: %d  (%s)\n", v_right, pwm_right, v_right >= 0 ? "PRZOD" : "TYL");
  Serial.printf("  v_left:  %+.3f m/s  ->  PWM: %d  (%s)\n", v_left,  pwm_left,  v_left  >= 0 ? "PRZOD" : "TYL");

  rotateMotor("right", pwm_right, v_right >= 0);
  rotateMotor("left",  pwm_left,  v_left  >= 0);
}

void onReceive(const esp_now_recv_info_t* info, const uint8_t* data, int len) {
  if (len != sizeof(DrivePacket)) {
    Serial.printf("[BLAD] Zly rozmiar pakietu: %d bajtow\n", len);
    return;
  }

  DrivePacket packet;
  memcpy(&packet, data, sizeof(packet));
  drive(packet.v, packet.omega);
}

void setup() {
  Serial.begin(115200);

  pinMode(L_PWM_1, OUTPUT);
  pinMode(R_PWM_1, OUTPUT);
  pinMode(L_PWM_2, OUTPUT);
  pinMode(R_PWM_2, OUTPUT);

  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(onReceive);

  Serial.println("Odbiornik gotowy. MAC:");
  Serial.println(WiFi.macAddress());
}

void loop() { }