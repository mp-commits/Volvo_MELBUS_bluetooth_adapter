/**
 * @file cd_changer.h
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CD_CHANGER_H
#define CD_CHANGER_H

typedef enum {
    MELBUS_MAIN_INIT,
    MELBUS_SECONDARY_INIT,
    MELBUS_CDCHGR_ID_REQ,
    MELBUS_TRACK_INFO_REQ,
    MELBUS_CARDRIDGE_INFO_REQ,
    MELBUS_POWER_UP,
    MELBUS_POWER_DOWN,
    MELBUS_DISC_NEXT,
    MELBUS_DISC_PREVIOUS,
    MELBUS_TRACK_NEXT,
    MELBUS_TRACK_PREVIOUS,
    MELBUS_RANDOM,
    MELBUS_FAST_FORWARD,
    MELBUS_FAST_REVERSE,
    MELBUS_UNKNOWN_COMMAND
} MELBUS_command_t;

void CD_CHANGER_Task();

void CD_CHANGER_Init();

#endif