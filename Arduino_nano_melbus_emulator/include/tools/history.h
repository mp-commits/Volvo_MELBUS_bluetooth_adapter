/**
 * @file history.h
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef HISTORY_H
#define HISTORY_H

#include "melbus.h"

#define HISTORY_SIZE 8

class ByteHistory
{
public:
    ByteHistory();
    uint32_t GetSize();
    MELBUS_byte_t GetByteAt(uint32_t location);
    bool Append(const MELBUS_byte_t byte);
    void Clear(const MELBUS_byte_t clearValue);
private:
    MELBUS_byte_t m_history[HISTORY_SIZE];
};

#endif