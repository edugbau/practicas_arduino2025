const int ledRojo = 3;
const int ledVerde = 4;

unsigned long periodoRojo = 1024;
unsigned long periodoVerde = 1458;

unsigned long tiempoAnteriorRojo = 0;
unsigned long tiempoAnteriorVerde = 0;

bool estadoRojo = false;
bool estadoVerde = false;

void setup() {
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
}

void loop() {
  unsigned long tiempoActual = millis();

  // LED Rojo
  if (tiempoActual - tiempoAnteriorRojo >= periodoRojo / 2) {
    estadoRojo = !estadoRojo;
    digitalWrite(ledRojo, estadoRojo ? HIGH : LOW);
    tiempoAnteriorRojo = tiempoActual;
  }

  // LED Verde
  if (tiempoActual - tiempoAnteriorVerde >= periodoVerde / 2) {
    estadoVerde = !estadoVerde;
    digitalWrite(ledVerde, estadoVerde ? HIGH : LOW);
    tiempoAnteriorVerde = tiempoActual;
  }
}
