/* - - PINES - - */
const int PIN_LED_ROJO = 3;
const int PIN_LED_VERDE = 5;
const int PIN_PNP = 2;
const int PIN_TEMP = 0;
/* - - TEMPERATURA - - */
const int TEMP_MAX = 40;
const int TEMP_MIN = 36;
const int TEMP_PIV = 38;
/* - - TIEMPOS - - */ 
const int FREQ_ACT_VERDE = 1000;
const int FREQ_CONTROL = 5000;
/* - - PARÁMETROS - - */

unsigned long tiempo_control;
unsigned long tiempo_led_verde;
unsigned long tiempoPeriodoError;
bool estadoError = false;

float tempActual;
int valorTmp;

float calc_temp(int valorTmp) {
  float Rt = (1023.0 * 10000.0 / valorTmp) - 10000.0;
  float T0 = 25.0 + 273.15;
  float R0 = 10000.0;
  float B = 3977.0;
  float tempK = 1.0 / ( (1.0 / T0) + (1.0 / B) * log(Rt / R0) );
  float tempC = tempK - 273.15;
  return tempC;
}

void setup_pin_leds(){
  pinMode(PIN_LED_ROJO, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_PNP, OUTPUT);
}

void setup_sistema(){
  valorTmp = analogRead(PIN_TEMP);
  tempActual = calc_temp(valorTmp);
  
  Serial.println("#> Arduino Control de Temperatura Basico (5000ms)");
  
  if( tempActual < TEMP_PIV ){
    digitalWrite(PIN_LED_ROJO,HIGH);
    digitalWrite(PIN_PNP,HIGH);
    Serial.print(tempActual);
    Serial.println(" 1");
  }else{
    digitalWrite(PIN_LED_ROJO,LOW);
    digitalWrite(PIN_PNP,LOW);
    Serial.print(tempActual);
    Serial.println(" 0");
  }

  if( tempActual < TEMP_MIN || tempActual > TEMP_MAX ){
    digitalWrite(PIN_LED_VERDE, LOW);
  }else{
    digitalWrite(PIN_LED_VERDE, HIGH);
  }

  unsigned long t_actual = millis();
  tiempo_control = t_actual;
  tiempo_led_verde = t_actual;
}

void setup (){
  Serial.begin(9600);
  while( !Serial ){
  } 
  
  setup_pin_leds();
  setup_sistema();
  
}

void loop () {
  unsigned long t_actual = millis ();
  valorTmp = analogRead(PIN_TEMP);
  tempActual = calc_temp(valorTmp);
  
  if( t_actual - tiempo_control > FREQ_CONTROL ){
    float error = TEMP_PIV - tempActual;

    if( error <= 0){
      digitalWrite(PIN_LED_ROJO,LOW);
      digitalWrite(PIN_PNP,LOW);
      Serial.print(tempActual);
      Serial.println(" 0");
    } else if (error < 5){
      estadoError = true;
      tiempoPeriodoError = t_actual + 1000 * error;

      digitalWrite(PIN_LED_ROJO,HIGH);
      digitalWrite(PIN_PNP,HIGH);
      Serial.print(tempActual);
      Serial.println(" 1");
    }else{
      digitalWrite(PIN_LED_ROJO,HIGH);
      digitalWrite(PIN_PNP,HIGH);
      Serial.print(tempActual);
      Serial.println(" 1");
    }
    
    tiempo_control = t_actual;
  } else if (estadoError && t_actual > tiempoPeriodoError){
    estadoError = false;
    digitalWrite(PIN_LED_ROJO,LOW);
    digitalWrite(PIN_PNP,LOW);
    Serial.print(tempActual);
    Serial.println(" 0");
  }

  if( t_actual - tiempo_led_verde > FREQ_ACT_VERDE ) {
    
    if( tempActual < TEMP_MIN || tempActual > TEMP_MAX ){
      digitalWrite(PIN_LED_VERDE, LOW);
    }else{
      digitalWrite(PIN_LED_VERDE, HIGH);
    }

    tiempo_led_verde = t_actual;

  }
}
