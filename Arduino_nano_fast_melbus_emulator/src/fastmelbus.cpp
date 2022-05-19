/**
 * @file fastmelbus.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "fastmelbus.h"
#include "Arduino.h"
#include "media_interface.h"

const uint8_t MELBUS_CLOCKBIT_INT = 0;
const uint8_t MELBUS_CLOCKBIT = 2;
const uint8_t MELBUS_DATA = 3;
const uint8_t MELBUS_BUSY = 4;

volatile uint8_t melbus_ReceivedByte = 0;
volatile uint8_t melbus_CharBytes = 0;
volatile uint8_t melbus_OutByte = 0xFF;
volatile uint8_t melbus_LastReadByte[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
volatile uint8_t melbus_SendBuffer[9] = {0x00,0x02,0x00,0x01,0x80,0x01,0xC7,0x0A,0x02};
volatile uint8_t melbus_SendCnt=0;
volatile uint8_t melbus_DiscBuffer[6] = {0x00,0xFC,0xFF,0x4A,0xFC,0xFF};
volatile uint8_t melbus_DiscCnt=0;
volatile uint8_t melbus_Bitposition = 0x80;

volatile bool InitialSequence_ext = false;
volatile bool sending_byte = false;
volatile bool melbus_MasterRequested = false;
volatile bool melbus_MasterRequestAccepted = false;
volatile bool f_mediaPaused;

static MediaInterface f_interface;
static MEDIA_Command_e f_command;
static MEDIA_Info_e f_info;

void melbus_Init_CDCHRG();
void MELBUS_CLOCK_INTERRUPT();
uint8_t FixTrackNumber(uint8_t number);

void FASTMELBUS_Init(void)
{
    f_mediaPaused = false;
    f_command = MEDIA_NO_COMMAND;
    f_info = MEDIA_INFO_NO_INFO;
    
    pinMode(MELBUS_DATA, INPUT_PULLUP);
    attachInterrupt(MELBUS_CLOCKBIT_INT, MELBUS_CLOCK_INTERRUPT, FALLING); 
    pinMode(MELBUS_CLOCKBIT, INPUT_PULLUP);
    melbus_Init_CDCHRG();
}

void FASTMELBUS_Task(void)
{
    if (digitalRead(MELBUS_BUSY)==HIGH) 
    {
        melbus_Bitposition = 0x80;
        melbus_OutByte = 0xFF;
        melbus_SendCnt=0;
        melbus_DiscCnt=0;
        DDRD &= ~(1<<MELBUS_DATA);
        PORTD |= (1<<MELBUS_DATA);

        if (f_command != MEDIA_NO_COMMAND)
        {
            f_interface.SendCommand(f_command);
            f_command = MEDIA_NO_COMMAND;
        }
        if (f_info != MEDIA_INFO_NO_INFO)
        {
            f_interface.SendInfoMessage(f_info);
            f_info = MEDIA_INFO_NO_INFO;
        }
    }  


    if ((melbus_Bitposition == 0x80) && (PIND & (1<<MELBUS_CLOCKBIT)))
    {
        delayMicroseconds(7);
        DDRD &= ~(1<<MELBUS_DATA);
        PORTD |= (1<<MELBUS_DATA);
    }
}

void melbus_Init_CDCHRG() {
    EIMSK &= ~(1<<INT0); 
    
    while(digitalRead(MELBUS_BUSY)==LOW){} 
    delayMicroseconds(10);
    
    pinMode(MELBUS_BUSY, OUTPUT);
    digitalWrite(MELBUS_BUSY, LOW);
    delay(1200);
    digitalWrite(MELBUS_BUSY, HIGH);
    pinMode(MELBUS_BUSY, INPUT_PULLUP);

    EIMSK |= (1<<INT0); 
}

void MELBUS_CLOCK_INTERRUPT() {
    if(melbus_OutByte & melbus_Bitposition)
    {
        DDRD &= (~(1<<MELBUS_DATA));
        PORTD |= (1<<MELBUS_DATA);
    }
    else{
        PORTD &= (~(1<<MELBUS_DATA));
        DDRD |= (1<<MELBUS_DATA);
    }
      
 
    if (PIND & (1<<MELBUS_DATA))
    {
        melbus_ReceivedByte |= melbus_Bitposition;
    }
    else 
    {
        melbus_ReceivedByte &=~melbus_Bitposition;
    }
    
    if (melbus_Bitposition==0x01) {
        for(int i=11;i>0;i--){
            melbus_LastReadByte[i] = melbus_LastReadByte[i-1];
        }

        if (melbus_OutByte != 0xFF)
        {
            melbus_LastReadByte[0] = melbus_OutByte;
            melbus_OutByte = 0xFF;
        } else 
        {
            melbus_LastReadByte[0] = melbus_ReceivedByte;
        }

        melbus_Bitposition=0x80;
        if(melbus_LastReadByte[2] == 0x07 && (melbus_LastReadByte[1] == 0x1A || melbus_LastReadByte[1] == 0x4A) && melbus_LastReadByte[0] == 0xEE)
        {
        InitialSequence_ext = true;
        f_info = MEDIA_INFO_INIT_BEGIN;
        }
        else if(melbus_LastReadByte[2] == 0x0 && (melbus_LastReadByte[1] == 0x1C || melbus_LastReadByte[1] == 0x4C) && melbus_LastReadByte[0] == 0xED)
        {
        InitialSequence_ext = true;
        f_info = MEDIA_INFO_INIT_BEGIN;
        }
        else if((melbus_LastReadByte[0] == 0xE8 || melbus_LastReadByte[0] == 0xE9) && InitialSequence_ext == true){
        InitialSequence_ext = false;
        melbus_OutByte = 0xEE;
        f_info = MEDIA_INFO_INIT_DONE;
        }
        else if((melbus_LastReadByte[2] == 0xE8 || melbus_LastReadByte[2] == 0xE9) && (melbus_LastReadByte[1] == 0x1E || melbus_LastReadByte[1] == 0x4E) && melbus_LastReadByte[0] == 0xEF)
        {
        // CartInfo
        melbus_DiscCnt=6;
        }
        else if((melbus_LastReadByte[2] == 0xE8 || melbus_LastReadByte[2] == 0xE9) && (melbus_LastReadByte[1] == 0x19 || melbus_LastReadByte[1] == 0x49) && melbus_LastReadByte[0] == 0x22)
        {
        // Powerdown
            melbus_OutByte = 0x00; // respond to powerdown;
            melbus_SendBuffer[1]=0x02; // STOP
            melbus_SendBuffer[8]=0x02; // STOP

            if (!f_mediaPaused)
            {
                f_command = MEDIA_PAUSE;
                f_mediaPaused = true;
            }
        }
        else if((melbus_LastReadByte[2] == 0xE8 || melbus_LastReadByte[2] == 0xE9) && (melbus_LastReadByte[1] == 0x19 || melbus_LastReadByte[1] == 0x49) && melbus_LastReadByte[0] == 0x52)
        {
            // RND
            melbus_OutByte = 0x00;
            f_command = MEDIA_RANDOM;
        }
        else if((melbus_LastReadByte[2] == 0xE8 || melbus_LastReadByte[2] == 0xE9) && (melbus_LastReadByte[1] == 0x19 || melbus_LastReadByte[1] == 0x49) && melbus_LastReadByte[0] == 0x2E)
        {
            // RND
            melbus_OutByte = 0x00;
            f_command = MEDIA_SCAN_MODE;
        }
        else if((melbus_LastReadByte[2] == 0xE8 || melbus_LastReadByte[2] == 0xE9) && (melbus_LastReadByte[1] == 0x19 || melbus_LastReadByte[1] == 0x49) && melbus_LastReadByte[0] == 0x29)
        {
            // FF
            melbus_OutByte = 0x00;
            f_command = MEDIA_FAST_FORWARD;
        }
        else if((melbus_LastReadByte[2] == 0xE8 || melbus_LastReadByte[2] == 0xE9) && (melbus_LastReadByte[1] == 0x19 || melbus_LastReadByte[1] == 0x49) && melbus_LastReadByte[0] == 0x26)
        {
            // FR
            melbus_OutByte = 0x00;
            f_command = MEDIA_FAST_REVERSE;
        }
        else if((melbus_LastReadByte[2] == 0xE8 || melbus_LastReadByte[2] == 0xE9) && (melbus_LastReadByte[1] == 0x19 || melbus_LastReadByte[1] == 0x49) && melbus_LastReadByte[0] == 0x2F)
        {
            melbus_OutByte = 0x00; // respond to start;
            melbus_SendBuffer[1]=0x08; // START
            melbus_SendBuffer[8]=0x08; // START

            f_mediaPaused = false;
            f_command = MEDIA_PLAY;
        }
        else if((melbus_LastReadByte[3] == 0xE8 || melbus_LastReadByte[3] == 0xE9) && (melbus_LastReadByte[2] == 0x1A || melbus_LastReadByte[2] == 0x4A) && melbus_LastReadByte[1] == 0x50 && melbus_LastReadByte[0] == 0x01)
        {
        // D-
            f_command = MEDIA_PREVIOUS_DISC;
        }
        else if((melbus_LastReadByte[3] == 0xE8 || melbus_LastReadByte[3] == 0xE9) && (melbus_LastReadByte[2] == 0x1A || melbus_LastReadByte[2] == 0x4A) && melbus_LastReadByte[1] == 0x50 && melbus_LastReadByte[0] == 0x41)
        {
        // D+
            f_command = MEDIA_NEXT_DISC;
        }
        else if((melbus_LastReadByte[4] == 0xE8 || melbus_LastReadByte[4] == 0xE9) && (melbus_LastReadByte[3] == 0x1B || melbus_LastReadByte[3] == 0x4B) && melbus_LastReadByte[2] == 0x2D && melbus_LastReadByte[1] == 0x00 && melbus_LastReadByte[0] == 0x01)
        {
        // T-
            f_command = MEDIA_PREVIOUS_TRACK;
        }
        else if((melbus_LastReadByte[4] == 0xE8 || melbus_LastReadByte[4] == 0xE9) && (melbus_LastReadByte[3] == 0x1B || melbus_LastReadByte[3] == 0x4B) && melbus_LastReadByte[2] == 0x2D && melbus_LastReadByte[1] == 0x40 && melbus_LastReadByte[0] == 0x01)
        {
        // T+
            f_command = MEDIA_NEXT_TRACK;
        }
        else if((melbus_LastReadByte[4] == 0xE8 || melbus_LastReadByte[4] == 0xE9) && (melbus_LastReadByte[3] == 0x1B || melbus_LastReadByte[3] == 0x4B) && melbus_LastReadByte[2] == 0xE0  && melbus_LastReadByte[1] == 0x01 && melbus_LastReadByte[0] == 0x08 ){
        // Playinfo
            melbus_SendCnt=9;
        }     
        if (melbus_SendCnt) {
            melbus_OutByte=melbus_SendBuffer[9-melbus_SendCnt];
            melbus_SendCnt--;
        } else if (melbus_DiscCnt) {
            melbus_OutByte=melbus_DiscBuffer[6-melbus_DiscCnt];
            melbus_DiscCnt--;
        }
        } else {
        melbus_Bitposition>>=1;
        }
        EIFR |= (1 << INTF0);
}

uint8_t FixTrackNumber(uint8_t number)
{
    //cut out A-F in each nibble, and skip "00"
    uint8_t hn = number >> 4;
    uint8_t ln = number & 0xF;
    if (ln == 0xA) {
        ln = 0;
        hn += 1;
    }
    if (ln == 0xF) {
        ln = 9;
    }
    if (hn == 0xA) {
        hn = 0;
        ln = 1;
    }
    if ((hn == 0) && (ln == 0)) {
        ln = 0x9;
        hn = 0x9;
    }
    return ((hn << 4) + ln);    
}