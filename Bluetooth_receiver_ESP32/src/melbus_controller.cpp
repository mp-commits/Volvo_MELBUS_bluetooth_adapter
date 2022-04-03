#include "melbus_controller.h"
#include "Arduino.h"

MelbusController::MelbusController(MediaControl* control)
{
    m_mediaControl = control;
    m_initBeginTime_ms = millis();
}

MelbusController::~MelbusController()
{
    
}

void MelbusController::Task()
{
    if (m_mediaControl->IsMelbusInitDone())
    {
        return;
    }
    else
    {
        uint32_t time = millis();
        uint32_t accumulated = time - m_initBeginTime_ms;
        if (accumulated > MELBUS_INIT_TIMEOUT_MS)
        {
            Serial.printf("ERROR: MELBUS init not received in time. Trying reset!\n");
            // Melbus init has not been received, arduino needs to be reset
            SetEnable(false);
            Reset();
            delay(100);

            m_mediaControl->ResetMelbusInitDone();
            m_initBeginTime_ms = millis();

            SetEnable(true);
        }
    }
}

void MelbusController::Init()
{
    pinMode(ARDUINO_RESET_PIN, OUTPUT);
    pinMode(ARDUINO_ENABLE_PIN, OUTPUT);
    digitalWrite(ARDUINO_RESET_PIN, HIGH);
    digitalWrite(ARDUINO_ENABLE_PIN, HIGH);
}

void MelbusController::Reset()
{
    digitalWrite(ARDUINO_RESET_PIN, LOW);
    delay(200);
    digitalWrite(ARDUINO_RESET_PIN, HIGH);
}

void MelbusController::SetEnable(bool enable)
{
    if(enable)
    {
        digitalWrite(ARDUINO_ENABLE_PIN, LOW);
        m_initBeginTime_ms = millis();
    }
    else
    {
        digitalWrite(ARDUINO_ENABLE_PIN, HIGH);
    }
}
