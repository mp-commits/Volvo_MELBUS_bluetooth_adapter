#include <Arduino.h>
#include "cd_changer.h"
#include "enable.h"

#define LED_BLINK_COUNT 60000
#define LED_PIN 13

static uint32_t f_ledCounter;
static bool f_ledOn;

static void ToggleLed();

void setup() {
    f_ledOn = true;
    f_ledCounter = 0;
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    ENABLE_Init();
    ENABLE_WaitForEnable();

    Serial.begin(115200);
    CD_CHANGER_Init();

    digitalWrite(LED_PIN, LOW);
    f_ledOn = false;
}

void loop() {
    CD_CHANGER_Task();

    f_ledCounter++;
    if (f_ledCounter > LED_BLINK_COUNT)
    {
        f_ledCounter = 0;
        ToggleLed();
    }
}

void ToggleLed()
{
    if (f_ledOn)
    {
        digitalWrite(LED_PIN, LOW);
        f_ledOn = false;
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
        f_ledOn = true;
    }
}
