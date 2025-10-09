#ifndef DLED_H
#define DLED_H

class DLed {
public:
    DLed(int pin);
    void on();
    void off();
    
private:
    int pin;
};

#endif // DLED_H