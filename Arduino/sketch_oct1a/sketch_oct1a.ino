
int potenciometro;
int potenciaFinal;
void setup() {
  pinMode(3,OUTPUT);
}

void loop() {
  potenciometro = analogRead(0);
  potenciaFinal = potenciometro * 255/1023;
  analogWrite(3,potenciaFinal);
}
