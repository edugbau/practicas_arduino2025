int therm;
float tempC;

void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  therm = analogRead(A0); // Leer el valor del thermistor
  float Vout = therm * 5.0 / 1023.0;
  float Rt = (1023.0 * 10000.0 / therm) - 10000.0;

  // Parámetros del thermistor NTCLE100E3103JB0
  float T0 = 25.0 + 273.15;
  float R0 = 10000.0;
  float B = 3977.0;

  float tempK = 1.0 / (1.0 / T0 + (1.0 / B) * log(Rt / R0));
  tempC = tempK - 273.15;

  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.println(" °C");

  if (tempC >= 36.0 && tempC <= 40.0) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }

  delay(1000);
}
