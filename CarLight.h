#ifndef CAR_LIGHT_H
#define CAR_LIGHT_H

#include <FastLED.h>
#include "RC.h"

class CarLight
{
public:
    CarLight(const int pin, const double stepTime, const int ledCount, const CRGB lightColor);

    void step();

    void turnOn();
    void turnOff();

    void turnOnBrake();
    void turnOffBrake();

    void turnOnEmergencyBrake();
    void turnOffEmergencyBrake();

private:
    bool on;
    bool braking;
    bool emergencyBraking;

    CRGB* colors;
    RC* filters;
    double stepSize;
    int leds;

    CRGB color;

    /**
     * Position and filter for the on/off animation
     */
    double position;
    RC positionFilter;
};

#endif