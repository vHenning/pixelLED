#include "pixelIno.h"
#include "PixelLED.h"

PixelLED led(PixelLED::BLINKER, CRGB::DarkOrange);

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