/**
 * @file history.cpp
 * @author Mikael Penttinen (mikael.penttinen3@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "history.h"

ByteHistory::ByteHistory()
{
    Clear(0x00);
}

/**
 * @brief Return history size
 * 
 * @return uint32_t history size
 */
uint32_t ByteHistory::GetSize()
{
    return HISTORY_SIZE;
}

/**
 * @brief Return bytes from the history
 * 
 * @param location 0: Most recent
 * @return uint8_t Byte at the given location
 */
uint8_t ByteHistory::GetByteAt(uint32_t location)
{
    if (location < HISTORY_SIZE)
    {
        return m_history[location];
    }
    else
    {
        return 0x00;
    }
}

/**
 * @brief Append byte to history
 * 
 * @param byte 
 * @return true Operation succesfull
 * @return false Operation failed
 */
bool ByteHistory::Append(const uint8_t byte)
{
    for(int32_t i = ByteHistory::GetSize() - 2; i >= 0; i--)
    {
        uint8_t temp = m_history[i];
        m_history[i+1] = temp;
    }
    
    m_history[0] = byte;

    return true;
}

/**
 * @brief Clear history with a specific value
 * 
 * @param clearValue 
 */
void ByteHistory::Clear(const uint8_t clearValue)
{
    for(uint32_t i = 0; i < ByteHistory::GetSize(); i++)
    {
        m_history[i] = clearValue;
    }
}