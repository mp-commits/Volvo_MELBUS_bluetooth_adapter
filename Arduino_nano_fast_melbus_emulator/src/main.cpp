/* Melbus CDCHGR Emulator
 * Program that emulates the MELBUS communication from a CD-changer (CD-CHGR) in a Volvo V70 (HU-xxxx) to enable AUX-input through the 8-pin DIN-contact. 
 * This setup is using an Arduino Nano 5v clone
 * 
 * The HU enables the CD-CHGR in its source-menue after a successful initialization procedure is accomplished. 
 * The HU will remove the CD-CHGR everytime the car starts if it wont get an response from CD-CHGR (second init-procedure).
 * 
 * Karl Hagström 2015-11-04
 * mod by S. Zeller 2016-03-14
 * 
 * This project went realy smooth thanks to these sources: 
 * http://volvo.wot.lv/wiki/doku.php?id=melbus
 * https://github.com/festlv/screen-control/blob/master/Screen_control/melbus.cpp
 * http://forums.swedespeed.com/showthread.php?50450-VW-Phatbox-to-Volvo-Transplant-(How-To)&highlight=phatbox
 * 
 * pulse train width=120us (15us per clock cycle), high phase between two pulse trains is 540us-600us 
 */

#include "Arduino.h"
#include "fastmelbus.h"
#include "enable.h"

#define LED_BLINK_COUNT 60000
#define LED_PIN 13

static uint32_t f_ledCounter;
static bool f_ledOn;

static void ToggleLed();

void setup() {
    Serial.begin(230400);
    ENABLE_Init();
    ENABLE_WaitForEnable();

    FASTMELBUS_Init();
}

void loop() {
    FASTMELBUS_Task();
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