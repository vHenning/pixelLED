#include "PixelLED.h"
#include "Colors.hpp"


PixelLED::PixelLED(Mode lightMode, CRGB color)
    : mode(lightMode)
    , color(color)
    , switchOn(false)
{
    for (size_t i = 0; i < LED_COUNT; ++i)
    {
        colors[i] = CRGB::Black;
        filters[i] = RC(0.01, 200, STEP);
    }

    FastLED.addLeds<WS2812B, LED_DRIVER_PIN, GRB>(colors, LED_COUNT);
}

void PixelLED::step()
{
    switch(mode)
    {
        case BLINKER:
        blinkerStep();
        break;
        case RUNNING_LIGHT:
        runningLightStep();
        break;
        case TEST:
        testStep();
        break;
        case TEST_SMOOTH:
        testSmoothStep();
        break;
    }

    FastLED.show();
}

void PixelLED::runningLightStep()
{
    static int counter = 0;
    if (++counter >= LED_COUNT)
    {
        counter = 0;
    }

    colors[counter] = color;

    int previous = counter == 0 ? LED_COUNT - 1 : counter - 1;

    colors[previous] = COLOR32_BLACK;
}

void PixelLED::blinkerStep()
{
    static int counter = 0;

    if (counter++ >= LED_COUNT - 1)
    {
        counter = 0;

        for (size_t i = 0; i < LED_COUNT; ++i)
        {
            colors[i] = COLOR32_BLACK;
        }
    }

    colors[counter] = color;
}

void PixelLED::testStep()
{
    static int counter = 0;
    static int stepCounter = 0;

    const int MILLIS_STEP = 500;
    if (STEP * stepCounter++ * 1000 > MILLIS_STEP)
    {
        stepCounter = 0;
        counter++;
    }
    if (counter > 2)
    {
        counter = 0;
    }

    for (size_t i = 0; i < LED_COUNT; ++i)
    {
        switch (counter)
        {
            case 0:
                colors[i] = CRGB::Red;
                break;
            case 1:
                colors[i] = CRGB::Green;
                break;
            case 2:
                colors[i] = CRGB::Blue;
                break;
            default:
                break;
        }
    }
}

void PixelLED::testSmoothStep()
{
    static int counter = 0;
    static bool on = true;

    if (counter++ > 1.0 / STEP)
    {
        counter = 0;
        on = !on;
    }

    ESP_Color::HSVf buffer;

    for (size_t i = 0; i < LED_COUNT; ++i)
    {
        // buffer = colors[i].ToHsv();
        // colors[i] = ESP_Color::Color::FromHsv(buffer);
        Serial.print("LED Value: ");
        Serial.println(buffer.V);
    }
}

void PixelLED::on()
{
    switchOn = true;
}

void PixelLED::off()
{
    switchOn = false;
}

void PixelLED::onOffStep()
{
    // Position and its derivations
    // position = 0 is at the edges of the strip. Center is LED_COUNT / 2
    static int position = 0;
    static float speed = 0;
    static float acceleration = 0;
    
    // Constant surge (derivation of acceleration)
    const float surge = 1;

    float currentSurge = 0;
}