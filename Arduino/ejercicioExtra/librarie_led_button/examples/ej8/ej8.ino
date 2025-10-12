volatile bool estadoLEDrojo = false;  
volatile bool estadoLEDverde = false;

DLed ledRojo(3);
DLed ledVerde(4);
Boton boton(2, 200);

void setup() {
  ledRojo.off();
  ledVerde.off();
}

void loop() {
  boton.update();
  if (boton.isPressed()) {
    estadoLEDverde = !estadoLEDverde;
    if (estadoLEDverde) {
      ledVerde.on();
    } else {
      ledVerde.off();
    }
    estadoLEDrojo = !estadoLEDrojo;
    if (estadoLEDrojo) {
      ledRojo.on();
    } else {
      ledRojo.off();
    }
  }
}