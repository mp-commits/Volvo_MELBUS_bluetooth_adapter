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

#include "stdint.h"

#define HISTORY_SIZE 3

class ByteHistory
{
public:
    ByteHistory();
    uint32_t GetSize();
    uint8_t GetByteAt(uint32_t location);
    bool Append(const uint8_t byte);
    void Clear(const uint8_t clearValue);
private:
    uint8_t m_history[HISTORY_SIZE];
};

#endif