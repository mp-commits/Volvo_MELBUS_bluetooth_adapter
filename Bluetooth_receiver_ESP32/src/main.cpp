#include <Arduino.h>
#include <project_pinconfig.h>
#include "media_interface.h"

#define PROJECT_SOFTWARE_VERSION "proto-0.1"

MediaInterface f_interface;

void setup() {
    Serial.begin(115200);
    Serial.printf("Begin init bt receiver ver: %s\n", PROJECT_SOFTWARE_VERSION);
    Serial.printf("Resetting arduino\n");

    pinMode(ARDUIONO_RESET_PIN, OUTPUT);
    digitalWrite(ARDUIONO_RESET_PIN, LOW);
    delay(500);
    digitalWrite(ARDUIONO_RESET_PIN, HIGH);

    Serial.printf("Init Done!\n");
}

void loop() {
    MEDIA_Command_e mediaCommand = f_interface.TryGetCommand();
    if (mediaCommand != MEDIA_NO_COMMAND)
    {
        Serial.printf("Command Received: %i", (uint8_t)(mediaCommand));
    }
}