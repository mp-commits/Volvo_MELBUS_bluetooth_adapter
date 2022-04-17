/**
 * @file media_control.cpp
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "media_control.h"
#include "Arduino.h"
#include "project_config.h"

MediaControl::MediaControl()
{
    m_melbusInitDone = false;
    m_debugEnabled = false;
    m_sink = nullptr;
    m_requestTime = millis();
}

void MediaControl::SetSink(BluetoothA2DPSink* sink)
{
    m_sink = sink;
}

MediaControl::~MediaControl()
{

}

bool MediaControl::IsMelbusInitDone()
{
    return m_melbusInitDone;
}

void MediaControl::ResetMelbusInitDone()
{
    m_melbusInitDone = false;
}

void MediaControl::Task()
{
    if (m_sink == nullptr)
    {
        return;
    }

    MEDIA_Command_e mediaCommand = m_interface.TryGetCommand();
    if (mediaCommand != MEDIA_NO_COMMAND)
    {
        if (m_debugEnabled)
        {
            m_interface.SerialPrintCommand(mediaCommand);
        }

            switch (mediaCommand)
        {
        case MEDIA_PLAY:
            m_sink->play();
            break;
        case MEDIA_PAUSE:
            m_sink->pause();
            break;
        case MEDIA_NEXT_TRACK:
            m_sink->next();
            break;
        case MEDIA_PREVIOUS_TRACK:
            m_sink->previous();
            break;
        case MEDIA_VOLUME_UP:
            m_sink->set_volume(m_sink->get_volume()+10);
            Serial.printf("DAC Vol set to %i\n", m_sink->get_volume());
            break;
        case MEDIA_VOLUME_DOWN:
            m_sink->set_volume(m_sink->get_volume()-10);
            Serial.printf("DAC Vol set to %i\n", m_sink->get_volume());
            break;
        case MEDIA_INFO_MELBUS_INIT_DONE:
            m_melbusInitDone = true;
            break;
        case MEDIA_RANDOM:
        default:
            break;
        }
    }

    uint32_t currentTime = millis();
    if (currentTime - m_requestTime >= MELBUS_STATUS_REQ_INTERVAL_MS)
    {
        m_requestTime = currentTime;
        if (!m_melbusInitDone)
        {
            m_interface.SendRequest(MEDIA_REQUEST_INIT_DONE);
        }
    }
}

void MediaControl::SetDebug(bool set)
{
    m_debugEnabled = set;
}