/**
 * @file project_pinconfig.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PROJECT_PINCONFIG_H
#define PROJECT_PINCONFIG_H

#define ARDUINO_RESET_PIN 17
#define ARDUINO_ENABLE_PIN 16
#define DAC_BCLK_PIN 33
#define DAC_DATA_PIN 25
#define DAC_WSL_PIN 26

#define MELBUS_INIT_TIMEOUT_MS 10000 // Allow 10 sec for melbus init before reset
#define MELBUS_STATUS_REQ_INTERVAL_MS 1000

#endif