#include <Arduino.h>
#include "melbus.h"

void setup() {
    Serial.begin(115200);
    Serial.print("MELBUS protocol sniffer v1\r\n");
    MELBUS_Init();
}

void loop() {
    bool dataread = false;

    if ( !MELBUS_IsBusy() )
    {
        while(MELBUS_RxBytesAvailable() > 0)
        {
            dataread = true;
            Serial.print(MELBUS_ReadByte(), HEX);
            Serial.print(" ");
        }
        if (dataread) 
        {
        Serial.print("\r\n");
        }
        dataread = false;
    }

    if (Serial.available())
    {
        char rx = Serial.read();

        switch (rx)
        {
        case 'i':
            MELBUS_BeginBusyInit();
            break;
        
        default:
            break;
        }
    }
    
}