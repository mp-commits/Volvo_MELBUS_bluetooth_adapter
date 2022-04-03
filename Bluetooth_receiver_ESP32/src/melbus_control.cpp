#include "melbus_controller.h"
#include "Arduino.h"

void MELBUSCONTROL_Init()
{
    pinMode(ARDUINO_RESET_PIN, OUTPUT);
    pinMode(ARDUINO_ENABLE_PIN, OUTPUT);
    digitalWrite(ARDUINO_RESET_PIN, HIGH);
    digitalWrite(ARDUINO_ENABLE_PIN, HIGH);
}

void MELBUSCONTROL_Reset()
{
    digitalWrite(ARDUINO_RESET_PIN, LOW);
    delay(100);
    digitalWrite(ARDUINO_RESET_PIN, HIGH);
}

void MELBUSCONTROL_SetEnable(bool enable)
{
    if(enable)
    {
        digitalWrite(ARDUINO_ENABLE_PIN, LOW);
    }
    else
    {
        digitalWrite(ARDUINO_ENABLE_PIN, HIGH);
    }
}
