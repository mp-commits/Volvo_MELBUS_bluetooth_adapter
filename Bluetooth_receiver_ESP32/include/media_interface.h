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
    MEDIA_UNKNOWN_COMMAND,
    MEDIA_NO_COMMAND,

    MEDIA_INFO_MELBUS_INIT_BEGIN,
    MEDIA_INFO_MELBUS_INIT_DONE,
    MEDIA_INFO_ALIVE,
} MEDIA_Command_e;

class MediaInterface {
public:
    MediaInterface();
    ~MediaInterface();

    MEDIA_Command_e TryGetCommand();
    void SerialPrintCommand(MEDIA_Command_e command);

private:
};

#endif