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
#define COM_HEADER  (uint8_t)(0xA0)
#define COM_UNKNOWN (uint8_t)(0x00)
#define COM_PLAY    (uint8_t)(0x01)
#define COM_PAUSE   (uint8_t)(0x02)
#define COM_NEXT    (uint8_t)(0x03)
#define COM_PREV    (uint8_t)(0x04)
#define COM_RANDOM  (uint8_t)(0x05)

static uint8_t EncodeCommand(MEDIA_Command_e cmd);
static char EncodeDebugCommand(MEDIA_Command_e cmd);

MediaInterface::MediaInterface()
{
    m_debugMode = false;
}

MediaInterface::~MediaInterface()
{

}

void MediaInterface::SendCommand(const MEDIA_Command_e command)
{
    if (m_debugMode)
    {
        Serial.println(EncodeDebugCommand(command), HEX);
    }
    else
    {
        Serial.write(EncodeCommand(command));
        // Serial.print(EncodeCommand(command));
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