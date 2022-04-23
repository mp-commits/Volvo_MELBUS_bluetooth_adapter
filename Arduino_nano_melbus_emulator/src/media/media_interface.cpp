/**
 * @file media_interface.cpp
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "media_interface.h"
#include "Arduino.h"
#include <stdint.h>

// Bytes for encoding commands
#define COM_HEADER          (uint8_t)(0xA0)
#define COM_UNKNOWN         (uint8_t)(0x00)
#define COM_PLAY            (uint8_t)(0x01)
#define COM_PAUSE           (uint8_t)(0x02)
#define COM_NEXT            (uint8_t)(0x03)
#define COM_PREV            (uint8_t)(0x04)
#define COM_RANDOM          (uint8_t)(0x05)
#define COM_VOL_UP          (uint8_t)(0x06)
#define COM_VOL_DOWN        (uint8_t)(0x07)
#define COM_FAST_FORWARD    (uint8_t)(0x08)
#define COM_FAST_REVERSE    (uint8_t)(0x09)
#define COM_SCAN_MODE       (uint8_t)(0x0A)

#define COM_INFO_HEADER     (uint8_t)(0xB0)
#define COM_INFO_UNKNOWN    (uint8_t)(0x00)
#define COM_INFO_INIT_BEGIN (uint8_t)(0x01)
#define COM_INFO_INIT_DONE  (uint8_t)(0x02)
#define COM_INFO_ALIVE      (uint8_t)(0x03)

static uint8_t EncodeInfoMessage(MEDIA_Info_e msg);
static uint8_t EncodeCommand(MEDIA_Command_e cmd);

static char EncodeDebugInfoMessage(MEDIA_Info_e msg);
static char EncodeDebugCommand(MEDIA_Command_e cmd);

MediaInterface::MediaInterface(): m_debugMode(false)
{
    
}

MediaInterface::~MediaInterface()
{

}

void MediaInterface::SendInfoMessage(const MEDIA_Info_e info)
{
    if(m_debugMode)
    {
        Serial.println(EncodeDebugInfoMessage(info));
    }
    else
    {
        Serial.write(EncodeInfoMessage(info));
    }
}

void MediaInterface::SendCommand(const MEDIA_Command_e command)
{
    if (m_debugMode)
    {
        Serial.println(EncodeDebugCommand(command));
    }
    else
    {
        Serial.write(EncodeCommand(command));
    }
}

void MediaInterface::SetDebugPrint(const bool set)
{
    m_debugMode = set;
}

uint8_t EncodeCommand(MEDIA_Command_e cmd)
{
    uint8_t commandByte = COM_UNKNOWN;

    switch (cmd)
    {
    case MEDIA_PLAY:
        commandByte = COM_PLAY;
        break;
    case MEDIA_PAUSE:
        commandByte = COM_PAUSE;
        break;
    case MEDIA_NEXT_TRACK:
        commandByte = COM_NEXT;
        break;
    case MEDIA_PREVIOUS_TRACK:
        commandByte = COM_PREV;
        break;
    case MEDIA_RANDOM:
        commandByte = COM_RANDOM;
        break;
    case MEDIA_VOLUME_UP:
        commandByte = COM_VOL_UP;
        break;
    case MEDIA_VOLUME_DOWN:
        commandByte = COM_VOL_DOWN;
        break;
    case MEDIA_FAST_FORWARD:
        commandByte = COM_FAST_FORWARD;
        break;
    case MEDIA_FAST_REVERSE:
        commandByte = COM_FAST_REVERSE;
        break;
    case MEDIA_SCAN_MODE:
        commandByte = COM_SCAN_MODE;
    default:
        break;
    }

    commandByte = commandByte | COM_HEADER;

    return commandByte;
}

char EncodeDebugCommand(MEDIA_Command_e cmd)
{
    char commandByte = 'u';

    switch (cmd)
    {
    case MEDIA_PLAY:
        commandByte = 'p';
        break;
    case MEDIA_PAUSE:
        commandByte = 's';
        break;
    case MEDIA_NEXT_TRACK:
        commandByte = 'n';
        break;
    case MEDIA_PREVIOUS_TRACK:
        commandByte = 'p';
        break;
    case MEDIA_RANDOM:
        commandByte = 'r';
        break;
    default:
        break;
    }

    return commandByte;
}

uint8_t EncodeInfoMessage(MEDIA_Info_e msg)
{
    uint8_t infoByte = COM_INFO_UNKNOWN;

    switch (msg)
    {
    case MEDIA_INFO_INIT_BEGIN:
        infoByte = COM_INFO_INIT_BEGIN;
        break;
    case MEDIA_INFO_INIT_DONE:
        infoByte = COM_INFO_INIT_DONE;
        break;
    case MEDIA_INFO_ALIVE:
        infoByte = COM_INFO_ALIVE;
        break;

    default:
        infoByte = COM_INFO_UNKNOWN;
        break;
    }

    infoByte = infoByte | COM_INFO_HEADER;

    return infoByte;
}

char EncodeDebugInfoMessage(MEDIA_Info_e msg)
{
    char infoByte = 'u';

    switch (msg)
    {
    case MEDIA_INFO_INIT_BEGIN:
        infoByte = 'i';
        break;
    case MEDIA_INFO_INIT_DONE:
        infoByte = 'd';
        break;

    default:
        infoByte = 'u';
        break;
    }

    return infoByte;
}