#pragma once

#include "Arduino.h"

class CC_STEC30
{
public:
    CC_STEC30();
    void begin();
    void attach();
    void detach();
    void set(int16_t messageID, char *setPoint);
    void update();

private:
    bool    _initialised;
    uint8_t _pin1, _pin2, _pin3;// State Variables
// LEDs
    bool altLedState = false,
     rdyLedState = false,
     lowVoltLedState = false,
     upLedState = false,
     downLedState = false,
     stLedState = false,
     hdLedState = false,
     trkLoLedState = false,
     trkHiLedState = false;

    // Turn Coordinator Needle
    double turnCoorNeedle = 50.0; // 0 to 100, 50 is centered.
    // Ball
    double inclinometerBall = 0.0;  // -100 to 100 with 0 being centered.
    void setTurnCoordNeedle(double percent);
    void setInclinometerBall(double percent);
    void setApTrimUpLight(bool state);
    void setApTrimDownLight(bool state);
    void setApTrkLoLight(bool state);
    void setApTrkHiLight(bool state);
    void setApStLight(bool state);
    void setApRdyLight(bool state);
    void setApHdLight(bool state);
    void setApAltLight(bool state);
    void setLowVoltLight(bool state);


    void displayLeds();
    void displayTurnCoordNeedle();
    void displayBall();
};