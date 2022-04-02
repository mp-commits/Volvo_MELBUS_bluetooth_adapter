/**
 * @file melbus.cpp
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "melbus.h"
#include <Arduino.h>
#include "tools/queue.h"

#define INT_NUM 0  

static MelbusQueue f_rxQueue;
static MelbusQueue f_txQueue;

static volatile MELBUS_byte_t f_rxByte;
static uint8_t f_rxByteBitIndex;

static void MelbusRxInterrupt();

bool MELBUS_Init()
{
    f_rxByteBitIndex = 7;
    f_rxByte = 0x00;

    pinMode(MELBUS_CLK_PIN, INPUT_PULLUP);
    // digitalWrite(MELBUS_CLK_PIN, HIGH);

    pinMode(MELBUS_BUSY_PIN, INPUT_PULLUP);
    // digitalWrite(MELBUS_BUSY_PIN, HIGH);

    pinMode(MELBUS_DATA_PIN, INPUT_PULLUP);
    // digitalWrite(MELBUS_DATA_PIN, HIGH);

    attachInterrupt(digitalPinToInterrupt(MELBUS_CLK_PIN), MelbusRxInterrupt, RISING);
    return true;
}

void MELBUS_BeginBusyInit(void)
{
    detachInterrupt(digitalPinToInterrupt(MELBUS_CLK_PIN));

    while(digitalRead(MELBUS_BUSY_PIN) == LOW);
    delayMicroseconds(10);
    pinMode(MELBUS_BUSY_PIN, OUTPUT);

    digitalWrite(MELBUS_BUSY_PIN, LOW);
    delay(1200);
    digitalWrite(MELBUS_BUSY_PIN, HIGH);

    pinMode(MELBUS_BUSY_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(MELBUS_CLK_PIN), MelbusRxInterrupt, RISING);
}

void MELBUS_SendByte(const MELBUS_byte_t byte)
{
    EIMSK &= ~(1 << INT_NUM);

    DDRD |= (1 << MELBUS_DATA_PIN);

    for (char i = 7; i >= 0; i--)
    {
        while (PIND & (1 << MELBUS_CLK_PIN)) {} 
        if (byte & (1 << i)) {
            PORTD |= (1 << MELBUS_DATA_PIN);
        }
        else {
            PORTD &= ~(1 << MELBUS_DATA_PIN);
        }
        while (!(PIND & (1 << MELBUS_CLK_PIN))) {}
    }
    delayMicroseconds(20);
    PORTD |= 1 << MELBUS_DATA_PIN;
    DDRD &= ~(1 << MELBUS_DATA_PIN);

    EIFR |= 1 << INT_NUM;
    EIMSK |= (1 << INT_NUM);
}

uint32_t MELBUS_RxBytesAvailable(void)
{
    return f_rxQueue.Count();
}

MELBUS_byte_t MELBUS_ReadByte(void)
{
    return f_rxQueue.Pop();
}

bool MELBUS_ConfirmSync(void)
{
    if(digitalRead(MELBUS_BUSY_PIN) == HIGH)
    {
        if(f_rxByteBitIndex != 7)
        {
            f_rxByteBitIndex = 7;
            return false;
        }
    }
    return true;
}

void MelbusRxInterrupt(void)
{
    if(digitalRead(MELBUS_DATA_PIN) == HIGH)
    {
        f_rxByte |= (1<<f_rxByteBitIndex);
    }
    else
    {
        f_rxByte &= ~(1<<f_rxByteBitIndex);
    }

    if (f_rxByteBitIndex == 0)
    {
        // Byte ready
        f_rxQueue.Push(f_rxByte);

        f_rxByteBitIndex = 7;
        f_rxByte = 0x00;
    }
    else
    {
        f_rxByteBitIndex--;
    }
}

