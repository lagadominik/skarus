// Definicje pinów ESP32 połączonych z DRV8825
const int dirPin = 26;   // Pin kierunku (DIR)
const int stepPin = 25;  // Pin kroku (STEP)

// Liczba kroków na jeden pełny obrót (200 dla silnika 1.8 stopnia)
const int stepsPerRevolution = 20000;

void setup() {
  // Konfiguracja pinów jako wyjścia
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Ustawienie kierunku obrotu (HIGH lub LOW)
  digitalWrite(dirPin, HIGH);

  // Krótkie opóźnienie przed startem
  delay(1000);

  // Pętla generująca impulsy (kroki)
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000); // Zmniejsz tę wartość, aby silnik kręcił się szybciej
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000); 
  }
}

void loop() {
  // Pusta pętla - silnik zakręci się tylko raz w sekcji setup()
  // Aby zakręcić ponownie, wciśnij przycisk RESET na ESP32
}