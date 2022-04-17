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
    MEDIA_VOLUME_UP,
    MEDIA_VOLUME_DOWN,
    MEDIA_UNKNOWN_COMMAND,
    MEDIA_NO_COMMAND,

    MEDIA_INFO_MELBUS_INIT_BEGIN,
    MEDIA_INFO_MELBUS_INIT_DONE,
    MEDIA_INFO_ALIVE,
} MEDIA_Command_e;

typedef enum {
    MEDIA_REQUEST_INIT_DONE,
} MEDIA_Request_e;

class MediaInterface {
public:
    MediaInterface();
    ~MediaInterface();

    MEDIA_Command_e TryGetCommand();
    void SerialPrintCommand(MEDIA_Command_e command);
    void SendRequest(MEDIA_Request_e request);

private:
};

#endif