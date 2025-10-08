#include <Arduino.h>

const int PIN_LED_ROJO = 3;
const int PIN_LED_VERDE = 5;
const int PIN_CALEFACTOR_PNP = 2;
const int PIN_SENSOR_TEMP = A0;

const int TEMP_MAX = 40;
const int TEMP_MIN = 36;
const int TEMP_PIVOTE = 38;

const int FREQ_CONTROL_MS = 5000;
const int FREQ_LED_VERDE_MS = 1000;

const float TEMP_NOMINAL = 25.0;
const float RESISTENCIA_NOMINAL = 10000.0;
const float COEFICIENTE_B = 3977.0;
const float RESISTENCIA_PULLUP = 10000.0;

unsigned long tiempoPrevioControl = 0;
unsigned long tiempoPrevioLedVerde = 0;
unsigned long tiempoFinPeriodoError = 0;
bool enPeriodoDeError = false;

float temperaturaActual = 0.0;

float calcularTemperatura(int valorAnalogico) {
  float resistencia = RESISTENCIA_PULLUP * (1023.0 / (float)valorAnalogico - 1.0);
  return (1.0 / ( (log(resistencia / RESISTENCIA_NOMINAL) / COEFICIENTE_B) + (1.0 / (TEMP_NOMINAL + 273.15)) )) - 273.15;
}

void imprimirEstadoSerial(float temp, bool estadoCalefactor) {
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(estadoCalefactor ? "1" : "0");
}

void actualizarEstadoCalefactor(bool encender) {
  digitalWrite(PIN_LED_ROJO, encender);
  digitalWrite(PIN_CALEFACTOR_PNP, encender);
  imprimirEstadoSerial(temperaturaActual, encender);
}

void actualizarLedDeRango() {
  bool enRango = (temperaturaActual >= TEMP_MIN && temperaturaActual <= TEMP_MAX);
  digitalWrite(PIN_LED_VERDE, enRango);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) { }

  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_CALEFACTOR_PNP, OUTPUT);

  temperaturaActual = calcularTemperatura(analogRead(PIN_SENSOR_TEMP));

  Serial.println("#> Arduino Control de Temperatura Basico");
  Serial.println("#$ -y20:45 -w4 -l36 -l38 -l40 -tTemperatura -tCalefactor");

  actualizarEstadoCalefactor(temperaturaActual < TEMP_PIVOTE);
  actualizarLedDeRango();

  unsigned long tiempoActual = millis();
  tiempoPrevioControl = tiempoActual;
  tiempoPrevioLedVerde = tiempoActual;
}

void loop() {
  unsigned long tiempoActual = millis();

  temperaturaActual = calcularTemperatura(analogRead(PIN_SENSOR_TEMP));

  if (tiempoActual - tiempoPrevioControl > FREQ_CONTROL_MS) {
    float error = TEMP_PIVOTE - temperaturaActual;

    if (error <= 0) {
      actualizarEstadoCalefactor(false);
    } else if (error < 5) {
      enPeriodoDeError = true;
      tiempoFinPeriodoError = tiempoActual + 1000 * error;
      actualizarEstadoCalefactor(false);
    } else {
      actualizarEstadoCalefactor(true);
    }
    tiempoPrevioControl = tiempoActual;

  } else if (enPeriodoDeError && tiempoActual > tiempoFinPeriodoError) {
    enPeriodoDeError = false;
    actualizarEstadoCalefactor(false);
  }

  if (tiempoActual - tiempoPrevioLedVerde > FREQ_LED_VERDE_MS) {
    actualizarLedDeRango();
    tiempoPrevioLedVerde = tiempoActual;
  }
}