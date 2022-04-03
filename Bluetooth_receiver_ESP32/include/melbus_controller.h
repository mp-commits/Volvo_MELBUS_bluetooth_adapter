/**
 * @file melbus_controller.h
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "project_pinconfig.h"
#include "Arduino.h"

extern void MELBUSCONTROL_Init();

extern void MELBUSCONTROL_Reset();

extern void MELBUSCONTROL_SetEnable(bool enable);
