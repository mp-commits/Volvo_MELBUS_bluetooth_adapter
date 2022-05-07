/**
 * @file queue.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "queue.h"

MelbusQueue::MelbusQueue()
{
    m_bytesAvailable = 0;
    m_readIndex = 0;
    m_writeIndex = 0;

    for(uint32_t i = 0; i < QUEUE_SIZE; i++)
    {
        m_buffer[i] = 0x00;
    }
}

void MelbusQueue::Push(const MELBUS_byte_t byte)
{
    m_buffer[m_writeIndex++] = byte;
    m_bytesAvailable++;

    if(m_writeIndex >= QUEUE_SIZE)
    {
        m_writeIndex = 0;
    }
    if(m_bytesAvailable > QUEUE_SIZE)
    {
        m_bytesAvailable = QUEUE_SIZE;
    }
}

MELBUS_byte_t MelbusQueue::Pop()
{
    MELBUS_byte_t returnByte = 0x00;
    if(m_bytesAvailable > 0)
    {
        returnByte = m_buffer[m_readIndex++];
        m_bytesAvailable--;

        if(m_readIndex >= QUEUE_SIZE)
        {
            m_readIndex = 0;
        }
    }

    return returnByte;
}

uint32_t MelbusQueue::Size()
{
    return QUEUE_SIZE;
}

uint32_t MelbusQueue::Count()
{
    return m_bytesAvailable;
}
