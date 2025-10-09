#ifndef BOTON_H
#define BOTON_H

class Boton {
public:
    Boton(int pin, unsigned long debounceTime);
    bool isPressed();
    void update();

private:
    int pin;
    unsigned long debounceTime;
    unsigned long lastDebounceTime;
    bool lastButtonState;
    bool buttonState;
};

#endif // BOTON_H