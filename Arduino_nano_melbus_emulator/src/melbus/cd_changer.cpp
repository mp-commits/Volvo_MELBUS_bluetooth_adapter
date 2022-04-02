/**
 * @file cd_changer.cpp
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "melbus.h"
#include "tools/history.h"
#include "cd_changer.h"
#include "media_interface.h"
#include <Arduino.h>

static uint8_t f_cdNumber = 1;
static uint8_t f_trackNumber = 1;

// RESPONSES
static const MELBUS_byte_t f_cdChangerId = 0xEE;
static MELBUS_byte_t f_trackInfo[9] = {0x00, 0x02, 0x00, f_cdNumber, 0x80, f_trackNumber, 0xC7, 0x0A, 0x02};
static const MELBUS_byte_t f_cardridgeInfo[6] = {0x00, 0xFC, 0xFF, 0x4A, 0xFC, 0xFF};

static bool f_initBegin;
static bool f_initDone;

static ByteHistory f_history;
static MediaInterface f_mediaInterface;

static MELBUS_command_t TryResolveCommand();
static void FixTrackNumber();

void CD_CHANGER_Task()
{
    if (MELBUS_RxBytesAvailable() > 0)
    {
        f_history.Append(MELBUS_ReadByte());
        MELBUS_command_t cmd = TryResolveCommand();

        switch (cmd)
        {
            case MELBUS_MAIN_INIT:
                f_initBegin = true;
                f_initDone = false;
                break;

            case MELBUS_SECONDARY_INIT:
                f_initBegin = true;
                f_initDone = false;
                break;

            case MELBUS_CDCHGR_ID_REQ:
                MELBUS_SendByte(f_cdChangerId);
                f_initDone = true;
                break;

            case MELBUS_TRACK_INFO_REQ:
                FixTrackNumber();
                f_trackInfo[3] = f_cdNumber;
                f_trackInfo[5] = f_trackNumber;
                for (uint32_t i = 0; i < 9; i++)
                {
                    MELBUS_SendByte(f_trackInfo[i]);
                }
                break;

            case MELBUS_CARDRIDGE_INFO_REQ:
                for (uint32_t i = 0; i < 6; i++)
                {
                    MELBUS_SendByte(f_cardridgeInfo[i]);
                }
                break;

            case MELBUS_POWER_UP:
                f_trackInfo[1] = 0x08;
                f_trackInfo[8] = 0x08;
                MELBUS_SendByte(0x00);
                f_mediaInterface.SendCommand(MEDIA_PLAY);
                break;

            case MELBUS_POWER_DOWN:
                f_trackInfo[1] = 0x02;
                f_trackInfo[8] = 0x02;
                MELBUS_SendByte(0x00);
                f_mediaInterface.SendCommand(MEDIA_PAUSE);
                break;

            case MELBUS_TRACK_NEXT:
                f_trackNumber++;
                f_mediaInterface.SendCommand(MEDIA_NEXT_TRACK);
                break;

            case MELBUS_TRACK_PREVIOUS:
                f_trackNumber--;
                f_mediaInterface.SendCommand(MEDIA_PREVIOUS_TRACK);
                break;

            case MELBUS_DISC_NEXT:
                f_cdNumber++;
                f_trackNumber = 1;
                break;

            case MELBUS_DISC_PREVIOUS:
                f_cdNumber--;
                f_trackNumber = 1;
                break;

            case MELBUS_RANDOM:
                f_mediaInterface.SendCommand(MEDIA_RANDOM);
                break;

            case MELBUS_FAST_FORWARD:
                break;

            case MELBUS_UNKNOWN_COMMAND:
            default:
                break;
        }
    }

    if(!MELBUS_ConfirmSync())
    {
    }
}

void CD_CHANGER_Init()
{
    f_initBegin = false;
    f_initDone = false;
    f_history.Clear(0x00);

    MELBUS_Init();
    MELBUS_BeginBusyInit();
}

#define HST(i) f_history.GetByteAt(i)

MELBUS_command_t TryResolveCommand()
{
    if(HST(2) == 0x07 && (HST(1) == 0x1A || HST(1) == 0x4A) && HST(0) == 0xEE)
    {
        return MELBUS_MAIN_INIT;
    }
    if (HST(2) == 0x0 && (HST(1) == 0x1C || HST(1) == 0x4C) && HST(0) == 0xED)
    {
        return MELBUS_SECONDARY_INIT;
    }
    if ((HST(0) == 0xE8 || HST(0) == 0xE9) && f_initBegin && !f_initDone)
    {
        return MELBUS_CDCHGR_ID_REQ;
    }
    if ((HST(4) == 0xE8 || HST(4) == 0xE9) && (HST(3) == 0x1B || HST(3) == 0x4B) && HST(2) == 0xE0  && HST(1) == 0x01 && HST(0) == 0x08)
    {
        return MELBUS_TRACK_INFO_REQ;
    }
    if ((HST(2) == 0xE8 || HST(2) == 0xE9) && (HST(1) == 0x1E || HST(1) == 0x4E) && HST(0) == 0xEF)
    {
        return MELBUS_CARDRIDGE_INFO_REQ;
    }
    if ((HST(2) == 0xE8 || HST(2) == 0xE9) && (HST(1) == 0x19 || HST(1) == 0x49) && HST(0) == 0x2F)
    {
        return MELBUS_POWER_UP;
    }
    if ((HST(2) == 0xE8 || HST(2) == 0xE9) && (HST(1) == 0x19 || HST(1) == 0x49) && HST(0) == 0x22)
    {
        return MELBUS_POWER_DOWN;
    }
    if ((HST(4) == 0xE8 || HST(4) == 0xE9) && (HST(3) == 0x1B || HST(3) == 0x4B) && HST(2) == 0x2D && HST(1) == 0x40 && HST(0) == 0x01)
    {
        return MELBUS_TRACK_NEXT;
    }
    if ((HST(4) == 0xE8 || HST(4) == 0xE9) && (HST(3) == 0x1B || HST(3) == 0x4B) && HST(2) == 0x2D && HST(1) == 0x00 && HST(0) == 0x01)
    {
        return MELBUS_TRACK_PREVIOUS;
    }
    if((HST(3) == 0xE8 || HST(3) == 0xE9) && (HST(2) == 0x1A || HST(2) == 0x4A) && HST(1) == 0x50 && HST(0) == 0x41)
    {
        return MELBUS_DISC_NEXT;
    }
    if((HST(3) == 0xE8 || HST(3) == 0xE9) && (HST(2) == 0x1A || HST(2) == 0x4A) && HST(1) == 0x50 && HST(0) == 0x01)
    {
        return MELBUS_DISC_PREVIOUS;
    }
    if((HST(2) == 0xE8 || HST(2) == 0xE9) && (HST(1) == 0x19 || HST(1) == 0x49) && HST(0) == 0x52)
    {
        return MELBUS_RANDOM;
    }
    if((HST(2) == 0xE8 || HST(2) == 0xE9) && (HST(1) == 0x19 || HST(1) == 0x49) && HST(0) == 0x29)
    {
        return MELBUS_FAST_FORWARD;
    }

    return MELBUS_UNKNOWN_COMMAND;
}

void FixTrackNumber()
{
    //cut out A-F in each nibble, and skip "00"
    byte hn = f_trackNumber >> 4;
    byte ln = f_trackNumber & 0xF;
    if (ln == 0xA) {
        ln = 0;
        hn += 1;
    }
    if (ln == 0xF) {
        ln = 9;
    }
    if (hn == 0xA) {
        hn = 0;
        ln = 1;
    }
    if ((hn == 0) && (ln == 0)) {
        ln = 0x9;
        hn = 0x9;
    }
    f_trackNumber = (hn << 4) + ln;    
}
