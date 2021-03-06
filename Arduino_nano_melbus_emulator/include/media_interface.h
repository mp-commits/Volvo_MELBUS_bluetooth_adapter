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
} MEDIA_Command_e;

typedef enum {
    MEDIA_INFO_INIT_BEGIN,
    MEDIA_INFO_INIT_DONE,
    MEDIA_INFO_ALIVE
} MEDIA_Info_e;

class MediaInterface {
public:
    MediaInterface();
    ~MediaInterface();

    void SendInfoMessage(const MEDIA_Info_e info);
    void SendCommand(const MEDIA_Command_e command);
    void SetDebugPrint(const bool set);

private:
    bool m_debugMode;
};

#endif
