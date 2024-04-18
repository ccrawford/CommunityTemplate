#pragma once

#include "Arduino.h"

class CC_STEC30
{
public:
    CC_STEC30();
    void begin();
    void attach(uint16_t Pin3, char *init);
    void detach();
    void set(int16_t messageID, char *setPoint);
    void update();

private:
    bool    _initialised;
    uint8_t _pin1, _pin2, _pin3;
};