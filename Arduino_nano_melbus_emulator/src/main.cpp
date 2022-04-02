#include <Arduino.h>
#include "cd_changer.h"

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Init");
    CD_CHANGER_Init();
}

void loop() {
    // put your main code here, to run repeatedly:
    CD_CHANGER_Task();
}