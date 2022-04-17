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

// Bytes for decoding commands
#define COM_HEADER   (uint8_t)(0xA0)
#define COM_UNKNOWN  (uint8_t)(0x00)
#define COM_PLAY     (uint8_t)(0x01)
#define COM_PAUSE    (uint8_t)(0x02)
#define COM_NEXT     (uint8_t)(0x03)
#define COM_PREV     (uint8_t)(0x04)
#define COM_RANDOM   (uint8_t)(0x05)
#define COM_VOL_UP   (uint8_t)(0x06)
#define COM_VOL_DOWN (uint8_t)(0x07)

#define COM_INFO_HEADER     (uint8_t)(0xB0)
#define COM_INFO_UNKNOWN    (uint8_t)(0x00)
#define COM_INFO_INIT_BEGIN (uint8_t)(0x01)
#define COM_INFO_INIT_DONE  (uint8_t)(0x02)
#define COM_INFO_ALIVE      (uint8_t)(0x03)

#define COM_REQ_HEADER      (uint8_t)(0xC0)
#define COM_REQ_INIT_DONE   (uint8_t)(0x01)

static MEDIA_Command_e TryDecodeCommand(const uint8_t byte);
static uint8_t EncodeRequest(const MEDIA_Request_e request);

MediaInterface::MediaInterface()
{

}

MediaInterface::~MediaInterface()
{

}

MEDIA_Command_e MediaInterface::TryGetCommand()
{
    MEDIA_Command_e command = MEDIA_NO_COMMAND;

    if (Serial.available() > 0)
    {
        uint8_t readByte = Serial.read();
        command = TryDecodeCommand(readByte);
    }

    return command;
}

void MediaInterface::SendRequest(MEDIA_Request_e request)
{
    uint8_t command = EncodeRequest(request);
    Serial.write(command);
}

MEDIA_Command_e TryDecodeCommand(const uint8_t byte)
{
    MEDIA_Command_e command = MEDIA_NO_COMMAND;

    const uint8_t checkByte = byte & 0xF0;
    if (checkByte == COM_HEADER)
    {
        const uint8_t commandByte = byte & 0x0F;
        switch (commandByte)
        {
        case COM_UNKNOWN:
            command = MEDIA_UNKNOWN_COMMAND;
            break;
        case COM_PLAY:
            command = MEDIA_PLAY;
            break;
        case COM_PAUSE:
            command = MEDIA_PAUSE;
            break;
        case COM_NEXT:
            command = MEDIA_NEXT_TRACK;
            break;
        case COM_PREV:
            command = MEDIA_PREVIOUS_TRACK;
            break;
        case COM_RANDOM:
            command = MEDIA_RANDOM;
            break;
        case COM_VOL_UP:
            command = MEDIA_VOLUME_UP;
            break;
        case COM_VOL_DOWN:
            command = MEDIA_VOLUME_DOWN;
            break;
        default:
            command = MEDIA_UNKNOWN_COMMAND;
            break;
        }
    }
    else if (checkByte == COM_INFO_HEADER)
    {
        const uint8_t commandByte = byte & 0x0F;
        switch (commandByte)
        {
        case COM_INFO_UNKNOWN:
            command = MEDIA_UNKNOWN_COMMAND;
            break;
        case COM_INFO_INIT_BEGIN:
            command = MEDIA_INFO_MELBUS_INIT_BEGIN;
            break;
        case COM_INFO_INIT_DONE:
            command = MEDIA_INFO_MELBUS_INIT_DONE;
            break;
        case COM_INFO_ALIVE:
            command = MEDIA_INFO_ALIVE;
            break;

        default:
            command = MEDIA_UNKNOWN_COMMAND;
            break;
        }
    }
    else
    {
        command = MEDIA_NO_COMMAND;
    }

    return command;
}

void MediaInterface::SerialPrintCommand(MEDIA_Command_e command)
{
    switch (command)
    {
    case MEDIA_PLAY:
        Serial.println("Mcmd: Play");
        break;
    case MEDIA_PAUSE:
        Serial.println("Mcmd: Pause");
        break;
    case MEDIA_NEXT_TRACK:
        Serial.println("Mcmd: Next");
        break;
    case MEDIA_PREVIOUS_TRACK:
        Serial.println("Mcmd: Prev");
        break;
    case MEDIA_RANDOM:
        Serial.println("Mcmd: Random");
        break;
    case MEDIA_VOLUME_UP:
        Serial.println("Mcmd: vol+");
        break;
    case MEDIA_VOLUME_DOWN:
        Serial.println("Mcmd: vol-");
        break;
    case MEDIA_UNKNOWN_COMMAND:
        Serial.println("Mcmd: Unknown");
        break;
    case MEDIA_INFO_MELBUS_INIT_BEGIN:
        Serial.println("Minfo: MELBUS init begin");
        break;
    case MEDIA_INFO_MELBUS_INIT_DONE:
        Serial.println("Minfo: MELBUS init done");
        break;
    case MEDIA_INFO_ALIVE:
        static uint32_t aliveCounter = 0;
        Serial.printf("Minfo: MELBUS alive, %i\n", aliveCounter++);
    default:
        break;
    }
}

uint8_t EncodeRequest(const MEDIA_Request_e request)
{
    uint8_t command = COM_REQ_HEADER;

    switch (request)
    {
    case MEDIA_REQUEST_INIT_DONE:
        command |= COM_REQ_INIT_DONE;
        break;
    
    default:
        break;
    }

    return command;
}
