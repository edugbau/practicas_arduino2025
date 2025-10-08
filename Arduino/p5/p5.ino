const int ledRojo = 3;
const int ledVerde = 4;

unsigned long periodoRojo = 5000;
unsigned long periodoVerde = 800;
unsigned long periodoSerial = 1000;

unsigned long tiempoAnteriorRojo = 0;
unsigned long tiempoAnteriorVerde = 0;
unsigned long tiempoAnteriorSerial = 0;

bool estadoRojo = false;
bool estadoVerde = false;

unsigned int valorIntensidad = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long tiempoActual = millis();
  // LED Verde
  if (tiempoActual - tiempoAnteriorVerde >= periodoVerde / 2) {
    estadoVerde = !estadoVerde;
    digitalWrite(ledVerde, estadoVerde ? HIGH : LOW);
    tiempoAnteriorVerde = tiempoActual;
  }
  // LED Rojo
  if(tiempoActual - tiempoAnteriorRojo >= periodoRojo){
    if (valorIntensidad >= 255){
      valorIntensidad = 0;
    }
    analogWrite(ledRojo, valorIntensidad);
    valorIntensidad += 255/5;
    tiempoAnteriorRojo = tiempoActual;
  }
  //Serial
  if (tiempoActual - tiempoAnteriorSerial >= periodoSerial){
    Serial.print("Estado del LED rojo: ");
    Serial.println(estadoRojo ? "HIGH\n" : "LOW");
    tiempoAnteriorSerial = tiempoActual;
  }

}
