int therm;
int potenciaFinal;

void setup() {
  pinMode(3, OUTPUT);
}

void loop() {
  therm = analogRead(0); // Lee el valor del thermistor (0–1023)
  potenciaFinal = therm * 255 / 1023; // Escala a rango PWM (0–255)
  analogWrite(3, potenciaFinal); // Aplica PWM al pin 3
}
