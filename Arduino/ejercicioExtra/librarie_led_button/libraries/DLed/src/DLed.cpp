#include "DLed.h"

DLed::DLed(int pinLed) {
    pin = pinLed;
    pinMode(pin, OUTPUT);
    off();
}

void DLed::on() {
    digitalWrite(pin, HIGH);
}

void DLed::off() {
    digitalWrite(pin, LOW);
}