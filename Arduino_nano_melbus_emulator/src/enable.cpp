/**
 * @file enable.cpp
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "enable.h"
#include "Arduino.h"

#define ABS(x) ((x)<0 ? -(x) : (x))

void ENABLE_Init()
{
    pinMode(ENABLE_PIN, INPUT_PULLUP);
}

void ENABLE_WaitForEnable()
{
    const uint32_t beginTime_ms = millis();
    uint32_t accumulatedTime_ms = 0;
    while (accumulatedTime_ms < ENABLE_TIMEOUT_MS) 
    {
        delay(10);
        const uint32_t time = millis();
        accumulatedTime_ms = ABS(time - beginTime_ms);

        if (digitalRead(ENABLE_PIN) == LOW)
        {
            accumulatedTime_ms = ENABLE_TIMEOUT_MS + 100;
            break;
        }
    }
}
