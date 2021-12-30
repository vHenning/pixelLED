#ifndef PIXEL_LED_H
#define PIXEL_LED_H

#include "ESP_Color.h"
#include <FastLED.h>
#include "RC.h"

class PixelLED
{
public:
    enum Mode
    {
        RUNNING_LIGHT,
        BLINKER,
        TEST,
        TEST_SMOOTH,
        ELEGANT_LIGHT
    } mode;

    CRGB color;

    PixelLED(int pin, int ledCount, double step, Mode lightMode, CRGB color);

    void step();

    void on();
    void off();

private:
    CRGB* colors;
    RC* filters;
    double stepSize;
    int leds;

    bool switchOn;

    void runningLightStep();
    void blinkerStep();
    void testStep();
    void testSmoothStep();
    void onOffStep();
};

#endif