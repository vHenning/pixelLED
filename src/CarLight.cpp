#include "CarLight.h"
#include "colors/ColorConverter.h"
#include "colors/GammaCorrection.h"

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
    , brightness(0.0)
    , useFilter(true)
    , turnFilterOffAfterChange(false)
    , turnFilterOnAfterChange(false)
    , emergencyBrakeCounter(0.0)
    , indicatorCounter(0.0)
    , blinker(OFF)
    , indicatorOn(false)
    , turnOffBlinker(false)
    , police(false)
    , policeCounter(0)
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

    brightness = NORMAL_BRIGHTNESS;
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

    const double emergencyBrakeFrequency = 5; // Hz
    const double emergencyBrakeHalfPeriod = 1.0 / (emergencyBrakeFrequency * 2);
    if (emergencyBraking)
    {
        brightness = emergencyBrakeCounter++ * stepSize > emergencyBrakeHalfPeriod ? NORMAL_BRIGHTNESS : BRAKE_BRIGHTNESS;

        if (emergencyBrakeCounter * stepSize > 2 * emergencyBrakeHalfPeriod)
        {
            emergencyBrakeCounter = 0;
        }
    }

    const float indicatorTime = 0.5;
    if (indicatorCounter++ * stepSize > indicatorTime)
    {
        if (turnOffBlinker)
        {
            blinker = OFF;
        }
        indicatorOn = !indicatorOn;
        indicatorCounter = 0;
    }

    policeCounter++;

    for (size_t i = 0; i < leds; ++i)
    {
        bool illuminate = i < floor(position) || i > leds - ceil(position);
        double localBrightness = illuminate ? brightness : 0.0;
        hsv.v = useFilter ? filters[i].step(localBrightness) : localBrightness;
        if (turnFilterOnAfterChange)
        {
            useFilter = true;
            turnFilterOnAfterChange = false;
        }
        if (abs(hsv.v - localBrightness) < std::numeric_limits<double>::epsilon() && turnFilterOffAfterChange)
        {
            useFilter = false;
            turnFilterOffAfterChange = false;
        }

        ColorConverter::rgb rgb = ColorConverter::hsv2rgb(hsv);

        if (police)
        {
            int millis = policeCounter * stepSize * 1000;

            static bool on = false;
            static bool left = false;

            if (i == 0)
            {
                if (millis % 100 < stepSize)
                {
                    on = !on;
                }
                if (millis % 400 < stepSize)
                {
                    left = !left;
                }
            }

            if ((left && on && (i > 80 && i < 100)) || (!left && on && (i < 54 && i > 34)))
            {
                rgb.r = 0.0;
                rgb.g = 0.0;
                rgb.b = 1.0;
            }
        }

        if ((indicatorOn && (blinker == RIGHT || blinker == HAZARD) && i < leds * indicatorWidth)
        ||  (indicatorOn && (blinker == LEFT || blinker == HAZARD) && i > leds - (leds * indicatorWidth)))
        {
            rgb.r = 1.0;
            rgb.g = 1.0;
            rgb.b = 0.0;
        }

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

void CarLight::turnOnBrake()
{
    braking = true;
    brightness = BRAKE_BRIGHTNESS;
    useFilter = false;
}

void CarLight::turnOffBrake()
{
    braking = false;
    brightness = NORMAL_BRIGHTNESS;
    if (!emergencyBraking)
    {
        turnFilterOnAfterChange = true;
    }
}

void CarLight::turnOnEmergencyBrake()
{
    emergencyBraking = true;
    useFilter = false;
    emergencyBrakeCounter = 0;
}

void CarLight::turnOffEmergencyBrake()
{
    emergencyBraking = false;
    if (!braking)
    {
        turnFilterOnAfterChange = true;
    }
}

void CarLight::left()
{
    if (blinker == OFF)
    {
        indicatorCounter = 0;
    }
    blinker = LEFT;
    turnOffBlinker = false;
}

void CarLight::right()
{
    if (blinker == OFF)
    {
        indicatorCounter = 0;
    }
    blinker = RIGHT;
    turnOffBlinker = false;
}

void CarLight::indicatorOff()
{
    turnOffBlinker = true;
}

void CarLight::hazard()
{
    if (blinker == OFF)
    {
        indicatorCounter = 0;
    }
    blinker = HAZARD;
    turnOffBlinker = false;
}

void CarLight::policeOn()
{
    police = true;
}

void CarLight::policeOff()
{
    police = false;
}