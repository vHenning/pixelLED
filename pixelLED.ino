#include "PixelLED.h"

#define LED_PIN 12
#define LED_COUNT 144
#define STEP 0.01

PixelLED led(LED_PIN, LED_COUNT, STEP, PixelLED::BLINKER, CRGB::DarkOrange);

void setup()
{
    Serial.begin(115200);
    Serial.println("Initialized!");
}

void loop()
{
    led.step();
    
    delay(1000 * STEP);
}