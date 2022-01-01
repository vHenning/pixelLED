#include "PixelLED.h"
#include "Colors.hpp"
#include "ColorConverter.h"

PixelLED::PixelLED(int pin, int ledCount, double step, Mode lightMode, CRGB color)
    : mode(lightMode)
    , color(color)
    , switchOn(false)
    , stepSize(step)
    , positionFilter(step, 200, 0.001)
    , leds(ledCount)
    , colors(new CRGB[ledCount])
    , filters(new RC[ledCount])
{
    for (size_t i = 0; i < ledCount; ++i)
    {
        colors[i] = CRGB::Black;
        filters[i] = RC(stepSize, 100, 0.001);
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
        case RUNNING_LIGHT_SMOOTH:
        runningLightSmoothStep();
        break;
        case ELEGANT_LIGHT:
        onOffStep();
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

    colors[previous] = CRGB::Black;
}

void PixelLED::runningLightSmoothStep()
{
    static double position = 0;

    const double SPEED = 100; // LEDs per second

    position += SPEED * stepSize;
    if (position > leds)
    {
        position -= leds;
    }

    float max = (float) 0xFF;
    ColorConverter::rgb rgb;
    rgb.r = color.r / max;
    rgb.g = color.g / max;
    rgb.b = color.b / max;

    ColorConverter::hsv hsv = ColorConverter::rgb2hsv(rgb);

    for (size_t i = 0; i < leds; ++i)
    {
        double value = 0.0;
        if (i - (int) position < 5)
        {
            value = 1.0;
        }

        hsv.v = filters[i].step(value);

        ColorConverter::rgb rgb = ColorConverter::hsv2rgb(hsv);
        colors[i].r = gamma8[(int)(rgb.r * max)];
        colors[i].g = gamma8[(int)(rgb.g * max)];
        colors[i].b = gamma8[(int)(rgb.b * max)];
    }
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

    if (counter++ > 5.0 / stepSize)
    {
        counter = 0;
        on = !on;
    }

    float max = (float) 0xFF;
    ColorConverter::rgb rgb;
    rgb.r = color.r / max;
    rgb.g = color.g / max;
    rgb.b = color.b / max;

    ColorConverter::hsv hsv = ColorConverter::rgb2hsv(rgb);

    for (size_t i = 0; i < leds; ++i)
    {
        double filterValue = filters[i].step(on ? 1.0 : 0.0);
        hsv.v = filterValue;
        ColorConverter::rgb rgb = ColorConverter::hsv2rgb(hsv);
        colors[i].r = rgb.r * max;
        colors[i].g = rgb.g * max;
        colors[i].b = rgb.b * max;
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

void PixelLED::onOff()
{
    switchOn = !switchOn;
}

void PixelLED::onOffStep()
{
    // position = 0 is at the edges of the strip. Center is LED_COUNT / 2
    // double desiredPosition = switchOn ? (double)(leds) / 2.0 : 0;
    double desiredPosition = switchOn ? leds / 2.0 -1 : 0.0;
    
    double position = positionFilter.step(desiredPosition);
    // double position = desiredPosition;

    float max = (float) 0xFF;
    ColorConverter::rgb rgb;
    rgb.r = color.r / max;
    rgb.g = color.g / max;
    rgb.b = color.b / max;

    ColorConverter::hsv hsv = ColorConverter::rgb2hsv(rgb);

    for (size_t i = 0; i < leds; ++i)
    {
        bool pixelOn = i <= ceil(position) || leds - 1 - i <= ceil(position);

        double filterValue = filters[i].step(pixelOn ? 0.3 : 0.0);
        hsv.v = filterValue;
        ColorConverter::rgb rgb = ColorConverter::hsv2rgb(hsv);
        colors[i].r = gamma8[(int)(rgb.r * max)];
        colors[i].g = gamma8[(int)(rgb.g * max)];
        colors[i].b = gamma8[(int)(rgb.b * max)];
    }
}