#include <Arduino.h>

// PINES
const int PIN_LED_ROJO = 3;
const int PIN_LED_VERDE = 5;
const int PIN_CALEFACTOR_PNP = 2;
const int PIN_SENSOR_TEMP = A0;

// CONSTANTES DE TEMPERATURA
const int TEMP_MAX = 40;
const int TEMP_MIN = 36;
const double TEMP_PIVOTE = 38.0; // Setpoint o target_value

// CONSTANTES DE TIEMPO
const int FREQ_CONTROL_MS = 5000; // Esto es PERIOD_MS
const int FREQ_LED_VERDE_MS = 1000;

// PARÁMETROS DEL TERMISTOR
const float TEMP_NOMINAL = 25.0;
const float RESISTENCIA_NOMINAL = 10000.0;
const float COEFICIENTE_B = 3977.0;
const float RESISTENCIA_PULLUP = 10000.0;

// --- VARIABLES Y CONSTANTES PARA EL PID (según pseudocódigo) ---
double Kp = 2.0;
double Ki = 0.5;
double Kd = 1.0;

// Umbrales para la lógica de control
const int PID_UMBRAL_APAGADO = 0;
const int PID_UMBRAL_PROPORCIONAL = 25;

// Variables de estado del PID
long last_ms_PID;
double prev_error;
double integral;

// VARIABLES GLOBALES
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

  // --- INICIALIZACIÓN PID (según pseudocódigo) ---
  last_ms_PID = -FREQ_CONTROL_MS; // Para que la primera ejecución sea inmediata
  prev_error = 0;
  integral = 0;

  // Configuración inicial del sistema
  temperaturaActual = calcularTemperatura(analogRead(PIN_SENSOR_TEMP));
  Serial.println("#> Arduino Control de Temperatura con PID (pseudocódigo)");
  Serial.println("#$ -y20:45 -w4 -l36 -l38 -l40 -tTemperatura -tCalefactor");
  actualizarEstadoCalefactor(temperaturaActual < TEMP_PIVOTE);
  actualizarLedDeRango();
  tiempoPrevioLedVerde = millis();
}

void loop() {
  unsigned long curr_ms = millis();

  // ----- LÓGICA DE CONTROL DEL CALEFACTOR (ejecutada cada FREQ_CONTROL_MS) -----
  if (curr_ms - last_ms_PID >= FREQ_CONTROL_MS) {
    last_ms_PID += FREQ_CONTROL_MS; // Avanza el tiempo programado, previene deriva

    // Lectura de temperatura justo antes del cálculo
    temperaturaActual = calcularTemperatura(analogRead(PIN_SENSOR_TEMP));

    // --- CÁLCULO PID (exactamente como en el pseudocódigo) ---
    double error = TEMP_PIVOTE - temperaturaActual;
    integral = integral + (error * FREQ_CONTROL_MS * 1e-3); // 1e-3 para convertir ms a s
    double derivative = (error - prev_error) / (FREQ_CONTROL_MS * 1e-3);
    double salidaPID = Kp * error + Ki * integral + Kd * derivative;
    prev_error = error;

    // --- CONTROL PID (lógica de activación del calefactor) ---
    if (salidaPID <= PID_UMBRAL_APAGADO) {
      actualizarEstadoCalefactor(false);
    } else if (salidaPID < PID_UMBRAL_PROPORCIONAL) {
      enPeriodoDeError = true;
      tiempoFinPeriodoError = curr_ms + 100 * salidaPID; // El tiempo de espera depende de la salida PID
      actualizarEstadoCalefactor(false);
    } else {
      actualizarEstadoCalefactor(true);
    }

  } else if (enPeriodoDeError && curr_ms > tiempoFinPeriodoError) {
    enPeriodoDeError = false;
    actualizarEstadoCalefactor(false);
  }

  // ----- ACTUALIZACIÓN DEL LED DE RANGO (lógica independiente) -----
  if (curr_ms - tiempoPrevioLedVerde > FREQ_LED_VERDE_MS) {
    actualizarLedDeRango();
    tiempoPrevioLedVerde = curr_ms;
  }
}