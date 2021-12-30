#include "PixelLED.h"
#include "Colors.hpp"


PixelLED::PixelLED(int pin, int ledCount, double step, Mode lightMode, CRGB color)
    : mode(lightMode)
    , color(color)
    , switchOn(false)
    , stepSize(step)
    , leds(ledCount)
    , colors(new CRGB[ledCount])
    , filters(new RC[ledCount])
{
    for (size_t i = 0; i < ledCount; ++i)
    {
        colors[i] = CRGB::Black;
        filters[i] = RC(0.01, 200, stepSize);
    }

    switch (pin)
    {
        case 0: FastLED.addLeds<WS2812B, 0, GRB>(colors, ledCount); break;
        case 1: FastLED.addLeds<WS2812B, 1, GRB>(colors, ledCount); break;
        case 2: FastLED.addLeds<WS2812B, 2, GRB>(colors, ledCount); break;
        case 3: FastLED.addLeds<WS2812B, 3, GRB>(colors, ledCount); break;
        case 4: FastLED.addLeds<WS2812B, 4, GRB>(colors, ledCount); break;
        case 5: FastLED.addLeds<WS2812B, 5, GRB>(colors, ledCount); break;
        case 12: FastLED.addLeds<WS2812B, 12, GRB>(colors, ledCount); break;
        case 13: FastLED.addLeds<WS2812B, 13, GRB>(colors, ledCount); break;
        case 14: FastLED.addLeds<WS2812B, 14, GRB>(colors, ledCount); break;
        case 15: FastLED.addLeds<WS2812B, 15, GRB>(colors, ledCount); break;
        case 16: FastLED.addLeds<WS2812B, 16, GRB>(colors, ledCount); break;
        case 17: FastLED.addLeds<WS2812B, 17, GRB>(colors, ledCount); break;
        case 18: FastLED.addLeds<WS2812B, 18, GRB>(colors, ledCount); break;
        case 19: FastLED.addLeds<WS2812B, 19, GRB>(colors, ledCount); break;
        case 21: FastLED.addLeds<WS2812B, 21, GRB>(colors, ledCount); break;
        case 22: FastLED.addLeds<WS2812B, 22, GRB>(colors, ledCount); break;
        case 23: FastLED.addLeds<WS2812B, 23, GRB>(colors, ledCount); break;
        case 25: FastLED.addLeds<WS2812B, 25, GRB>(colors, ledCount); break;
        case 26: FastLED.addLeds<WS2812B, 26, GRB>(colors, ledCount); break;
        case 27: FastLED.addLeds<WS2812B, 27, GRB>(colors, ledCount); break;
        case 32: FastLED.addLeds<WS2812B, 32, GRB>(colors, ledCount); break;
        case 33: FastLED.addLeds<WS2812B, 33, GRB>(colors, ledCount); break;
        default: break;
    }
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
    if (++counter >= leds)
    {
        counter = 0;
    }

    colors[counter] = color;

    int previous = counter == 0 ? leds - 1 : counter - 1;

    // TODO
    colors[previous] = COLOR32_BLACK;
}

void PixelLED::blinkerStep()
{
    static int counter = 0;

    if (counter++ >= leds - 1)
    {
        counter = 0;

        for (size_t i = 0; i < leds; ++i)
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
    if (stepSize * stepCounter++ * 1000 > MILLIS_STEP)
    {
        stepCounter = 0;
        counter++;
    }
    if (counter > 2)
    {
        counter = 0;
    }

    for (size_t i = 0; i < leds; ++i)
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

    if (counter++ > 1.0 / stepSize)
    {
        counter = 0;
        on = !on;
    }

    ESP_Color::HSVf buffer;

    for (size_t i = 0; i < leds; ++i)
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