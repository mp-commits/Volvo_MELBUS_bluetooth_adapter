/**
 * @file media_control.h
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MEDIA_CONTROL_H
#define MEDIA_CONTROL_H

#include "media_interface.h"
#include "BluetoothA2DPSink.h"

class MediaControl {
public:
    MediaControl();
    ~MediaControl();

    void SetSink(BluetoothA2DPSink* sink);
    void Task();
    void SetDebug(bool set);
    bool IsMelbusInitDone();
    void ResetMelbusInitDone();
    void RandomNextTrack();

private:
    MediaInterface m_interface;
    BluetoothA2DPSink* m_sink;
    bool m_debugEnabled;
    bool m_melbusInitDone;
};

#endif