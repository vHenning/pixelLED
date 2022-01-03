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

    void left();
    void right();
    void indicatorOff();
    void hazard();

    void policeOn();
    void policeOff();

private:
    bool on;
    bool braking;
    bool emergencyBraking;

    CRGB* colors;
    RC* filters;
    double stepSize;
    int leds;

    CRGB color;
    double brightness;
    const double NORMAL_BRIGHTNESS = 0.3;
    const double BRAKE_BRIGHTNESS = 1.0;
    bool useFilter;
    bool turnFilterOnAfterChange;
    bool turnFilterOffAfterChange;

    int emergencyBrakeCounter;
    int indicatorCounter;
    bool indicatorOn;
    bool turnOffBlinker;

    unsigned int policeCounter;
    bool police;

    /**
     * percent of whole light strip
     */
    const float indicatorWidth = 0.2;

    enum Blinker
    {
        OFF,
        LEFT,
        RIGHT,
        HAZARD
    } blinker;

    /**
     * Position and filter for the on/off animation
     */
    double position;
    RC positionFilter;
};

#endif