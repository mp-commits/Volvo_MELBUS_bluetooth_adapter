/**
 * @file melbus.h
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief Main header for the melbus component
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MELBUS_H
#define MELBUS_H

#include <stdint.h>
#include <stdbool.h>

#define MELBUS_CLK_PIN 2
#define MELBUS_DATA_PIN 3
#define MELBUS_BUSY_PIN 4

typedef uint8_t MELBUS_byte_t;

bool MELBUS_Init();

void MELBUS_BeginBusyInit(void);

void MELBUS_SendByte(const MELBUS_byte_t byte);

uint32_t MELBUS_RxBytesAvailable(void);

MELBUS_byte_t MELBUS_ReadByte(void);

bool MELBUS_ConfirmSync(void);

#endif