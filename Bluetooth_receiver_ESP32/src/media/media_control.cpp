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

MediaControl::MediaControl()
{
    m_debugEnabled = false;
    m_sink = nullptr;
}

void MediaControl::SetSink(BluetoothA2DPSink* sink)
{
    m_sink = sink;
}

MediaControl::~MediaControl()
{

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
        case MEDIA_RANDOM:
        default:
            break;
        }
    }
}

void MediaControl::SetDebug(bool set)
{
    m_debugEnabled = set;
}