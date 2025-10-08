volatile bool estadoLEDrojo = false;  
volatile bool estadoLEDverde = false;

void cambiarLED() {
  bool estadoBoton = digitalRead(2);

  if (estadoBoton == HIGH) {
    estadoLEDverde = !estadoLEDverde;
    digitalWrite(4, estadoLEDverde);
    estadoLEDrojo = !estadoLEDrojo;
    digitalWrite(3, estadoLEDrojo);
  } else {
    estadoLEDrojo = !estadoLEDrojo;
    digitalWrite(3, estadoLEDrojo);
  }
}

void setup() {
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(2), cambiarLED, CHANGE);
}

void loop() {
}
