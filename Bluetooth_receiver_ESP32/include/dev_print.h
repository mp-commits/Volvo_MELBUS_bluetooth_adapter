/**
 * @file dev_print.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DEV_PRINT_H
#define DEV_PRINT_H

#include "Arduino.h"

extern bool DEV_PRINT_ENABLED;

#define PRINT(line) if (DEV_PRINT_ENABLED) Serial.print((line));
#define PRINTLN(line) if (DEV_PRINT_ENABLED) Serial.println((line));
#define PRINTF1(format, val) if (DEV_PRINT_ENABLED) {Serial.printf(format, val);}

#endif // DEV_PRINT_H