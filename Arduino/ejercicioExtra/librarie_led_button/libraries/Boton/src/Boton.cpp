#include "Boton.h"

Boton::Boton(int pin, unsigned long debounceTime) : pin(pin), debounceTime(debounceTime), lastButtonState(LOW), lastDebounceTime(0), buttonState(LOW) {
    pinMode(pin, INPUT_PULLUP);
}

void Boton::update() {
    int actualState = digitalRead(pin);
    unsigned long actualTime = millis();

    if (actualState != lastButtonState) {
        lastDebounceTime = actualTime;
    }

    if ((actualTime - lastDebounceTime) > debounceTime) {
        if (actualState != buttonState) {
            buttonState = actualState;
        }
    }

    lastButtonState = actualState;
}

bool Boton::isPressed() {
    return buttonState == LOW;
}