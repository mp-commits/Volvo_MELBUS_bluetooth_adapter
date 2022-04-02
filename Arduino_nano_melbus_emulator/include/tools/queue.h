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

#include "melbus.h"

#define QUEUE_SIZE 64

class MelbusQueue 
{
public:
    MelbusQueue();
    void Push(const MELBUS_byte_t byte);
    MELBUS_byte_t Pop();
    uint32_t Size();
    uint32_t Count();
private:
    MELBUS_byte_t m_buffer[QUEUE_SIZE];
    uint32_t m_readIndex;
    uint32_t m_writeIndex;
    uint32_t m_bytesAvailable;
};
