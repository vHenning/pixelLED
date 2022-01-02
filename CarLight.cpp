#include "CarLight.h"
#include "ColorConverter.h"
#include "GammaCorrection.h"

CarLight::CarLight(const int pin, const double stepTime, const int ledCount, const CRGB lightColor)
    : on(false)
    , braking(false)
    , emergencyBraking(false)
    , colors(new CRGB[ledCount])
    , filters(new RC[ledCount])
    , positionFilter(stepTime, 200, 0.001)
    , stepSize(stepTime)
    , leds(ledCount)
    , position(0)
    , color(lightColor)
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

void CarLight::step()
{
    position = positionFilter.step(on ? (leds / 2.0) + 1 : 0);
    
    ColorConverter::rgb rgb;
    double max = (double) 0xFF;
    rgb.r = color.r / max;
    rgb.g = color.g / max;
    rgb.b = color.b / max;
    ColorConverter::hsv hsv = ColorConverter::rgb2hsv(rgb);

    for (size_t i = 0; i < leds; ++i)
    {
        bool illuminate = i < floor(position) || i > leds - ceil(position);
        hsv.v = filters[i].step(illuminate ? 0.3 : 0.0);
        ColorConverter::rgb rgb = ColorConverter::hsv2rgb(hsv);
        colors[i].r = gamma8[(int)(rgb.r * max)];
        colors[i].g = gamma8[(int)(rgb.g * max)];
        colors[i].b = gamma8[(int)(rgb.b * max)];
    }

    FastLED.show();
}

void CarLight::turnOn()
{
    on = true;
}

void CarLight::turnOff()
{
    on = false;
}