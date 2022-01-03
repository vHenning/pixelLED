#include "PixelLED.h"
#include "CarLight.h"

#define LED_PIN 12
#define LED_COUNT 144
#define STEP 0.01

// PixelLED led(LED_PIN, LED_COUNT, STEP, PixelLED::ELEGANT_LIGHT, CRGB::Red);
CarLight light(LED_PIN, STEP, LED_COUNT, CRGB::Red);

void sleep(const double step, const unsigned long start);

void setup()
{
    Serial.begin(115200);
    Serial.println("Initialized!");

    light.turnOn();
}

void loop()
{
    const static double cycleTime = 5;
    static int cyclesSinceSwap = 0;
    ++cyclesSinceSwap;

    unsigned long start = micros();
    // led.step();
    light.step();

    if (cyclesSinceSwap > cycleTime / STEP)
    {
        // led.onOff();
        static bool on = true;
        on ? light.hazard() : light.indicatorOff();
        on = !on;
        cyclesSinceSwap = 0;
    }

    sleep(STEP, start);
}

void sleep(const double step, const unsigned long start)
{
    long duration = micros() - (long) start;


    int stepMicros = 1000000.0 * step;
    if (duration > stepMicros)
    {
        Serial.println("Lasted longer than allowed!");
        Serial.print("Duration ");
        Serial.print(duration);
        Serial.print(" Step ");
        Serial.println(stepMicros);
        return;
    }

    delayMicroseconds(step * 1000000 - duration);
}