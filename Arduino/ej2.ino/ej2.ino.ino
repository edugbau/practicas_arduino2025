void setup() {
  pinMode(2, OUTPUT); // Configura el pin 2 como salida
}

void loop() {
  digitalWrite(2, HIGH); // Enciende el LED
  delay(1000);           // Espera 1 segundo
  digitalWrite(2, LOW);  // Apaga el LED
  delay(1000);           // Espera 1 segundo
}