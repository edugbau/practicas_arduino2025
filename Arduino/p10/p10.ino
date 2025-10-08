
int ldr;
int potenciaFinal;
void setup() {
  pinMode(3,OUTPUT);
}

void loop() {
  ldr = analogRead(0);
  potenciaFinal = ldr * 255/10000;
  analogWrite(3,potenciaFinal);
}

