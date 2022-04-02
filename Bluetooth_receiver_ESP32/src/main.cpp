#include <Arduino.h>
#include <project_pinconfig.h>
#include "BluetoothA2DPSink.h"
#include "media_control.h"

#define PROJECT_SOFTWARE_VERSION "proto-0.1"

BluetoothA2DPSink f_bluetoothSink;
MediaControl f_control;

void setup() {
    Serial.begin(115200);
    Serial.printf("Begin init bt receiver ver: %s\n", PROJECT_SOFTWARE_VERSION);
    Serial.printf("Resetting arduino\n");

    pinMode(ARDUIONO_RESET_PIN, OUTPUT);
    digitalWrite(ARDUIONO_RESET_PIN, LOW);
    delay(500);
    digitalWrite(ARDUIONO_RESET_PIN, HIGH);
    delay(3000);

    Serial.printf("Setting up control\n");
    f_control.SetSink(&f_bluetoothSink);

    Serial.printf("Starting bluetooth sink\n");

    static const i2s_config_t i2s_config = {
         .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
         .sample_rate = 41000,
         .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
         .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
         .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
         .intr_alloc_flags = 0, // default interrupt priority
         .dma_buf_count = 8,
         .dma_buf_len = 64,
         .use_apll = false,
         .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
    };
    f_bluetoothSink.set_i2s_config(i2s_config);
    f_bluetoothSink.start("My test sink");

    Serial.printf("Init Done!\n");
}

void loop() {
    f_control.Task();
}