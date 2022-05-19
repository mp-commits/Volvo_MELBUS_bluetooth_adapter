/**
 * @file media_interface.h
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MEDIA_INTERFACE_H
#define MEDIA_INTERFACE_H

#include "stdint.h"
#include "history.h"

typedef enum {
    MEDIA_PLAY,
    MEDIA_PAUSE,
    MEDIA_NEXT_TRACK,
    MEDIA_PREVIOUS_TRACK,
    MEDIA_RANDOM,
    MEDIA_SCAN_MODE,
    MEDIA_NEXT_DISC,
    MEDIA_PREVIOUS_DISC,
    MEDIA_FAST_FORWARD,
    MEDIA_FAST_REVERSE,

    MEDIA_NO_COMMAND,
} MEDIA_Command_e;

typedef enum {
    MEDIA_DATA_NO_DATA,
    MEDIA_DATA_DISC_NUMBER,
    MEDIA_DATA_TRACK_NUMBER,
} MEDIA_Data_e;

typedef enum {
    MEDIA_INFO_INIT_BEGIN,
    MEDIA_INFO_INIT_DONE,
    MEDIA_INFO_ALIVE,

    MEDIA_INFO_NO_INFO,
} MEDIA_Info_e;

class MediaInterface {
public:
    MediaInterface();
    ~MediaInterface();

    void SendInfoMessage(const MEDIA_Info_e info);
    void SendCommand(const MEDIA_Command_e command);
    void SetDebugPrint(const bool set);

    MEDIA_Data_e TryGetData();

    uint8_t GetDiscNumber();
    uint8_t GetTrackNumber();

private:
    bool m_debugMode;
    uint8_t m_track;
    uint8_t m_disc;
    ByteHistory m_rxHistory;
};

#endif
