/**
 * @file enable.h
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ENABLE_H
#define ENABLE_H

#define ENABLE_TIMEOUT_MS 10000
#define ENABLE_PIN 5

extern void ENABLE_Init();
extern void ENABLE_WaitForEnable();

#endif