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

#ifndef MELBUS_CONTROLLER_H
#define MELBUS_CONTROLLER_H

#include "project_config.h"
#include "media_control.h"
#include "Arduino.h"

class MelbusController
{
public:
    MelbusController(MediaControl* control);
    ~MelbusController();
    void Init();
    void Reset();
    void SetEnable(bool enable);

    void Task();
private:
    MediaControl* m_mediaControl;
    uint32_t m_initBeginTime_ms;
};

#endif