// Definicje pinów ESP32 połączonych z DRV8825
const int dirPin = 26;   // Pin kierunku (DIR)
const int stepPin = 25;  // Pin kroku (STEP)

const int xPin = 24;
const int yPin = 23;
const int resolution = 4095;
const double rMax = res/2;
const double xy_ignore=0.1; //wspolczynnik ignorujacy male wychylenia na srodku i na osiach ox i oy
const double movement_ignore=0.1; //wspolczynnik rozpoznajacy sytuacje, gdy jeden silnik stoi
const double tMin=10;


// Liczba kroków na jeden pełny obrót (200 dla silnika 1.8 stopnia)
const int stepsPerRevolution = 200;

void timeCalculator(double &tL, double &tR, int &dirL, int &dirL)){ //wyznacza czas miedzy sygnalami do zrobienia kroku dla dwoch silnikow
  double x = analogRead(xPin) - rMax;
  double y = analogRead(yPin) - rMax;
  
  if (abs(x) <= k*rMax && abs(y) <= k*rMax){ //jesli blisko srodka, to nie ruszamy
    tL=-1;
    tR=-1;
  }else{
    if (abs(x) <= xy_ignore*rMax){ //jesli blisko ox to tylko do przodu
      dirL = (y>0 ? 1 : 0);
      dirR = (y>0 ? 0 : 1);
      tL = tR = tMin * sqrt(2) * rMax / abs(y);
    }else{
      if (abs(y) <= xy_ignore*rMax){ //jesli blisko oy to tylko obrot
        dirL = (x>0 ? 0 : 1);
        dirR = (x>0 ? 0 : 1);
        tL = tR = tMin * sqrt(2) * rMax / abs(x);
      }else{
        if(abs(x/y) <= 1+movement_ignore && abs(x/y) >= 1-movement_ignore){ // jesli jeden silnik ma sie nie ruszac (abs(y)==okolo abs(x))
          if(x*y>0){
            dirL = (y>0 ? 1 : 0);
            tR=-1;
            tL=tMin;
          }else{
            dirR = (y>0 ? 0 : 1);
            tL=-1;
            tR=tMin;
          }
        }else{ //w pozostalych przypadkach standardowe sterowanie
          dirL = (y+x>0 ? 1: 0);
          dirR = (y-x>0 ? 0 : 1);
          tL = tMin * sqrt(2) * rMax / abs(y+x);
          tR = tMin * sqrt(2) * rMax / abs(y-x);
        }
      }
    }
  }
}

//void engineMovement(double &tL, double &tR, int &dirL, int &dirL)){

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