#include <Arduino.h>
#include <project_pinconfig.h>
#include "BluetoothA2DPSink.h"
#include "melbus_controller.h"

#define PROJECT_SOFTWARE_VERSION "proto-0.5"

BluetoothA2DPSink f_bluetoothSink;
MediaControl f_mediaControl;
MelbusController f_melbusController(&f_mediaControl);

static void InitA2DPSink();

void setup() {
    Serial.begin(115200);
    Serial.printf("Begin init bt receiver ver: %s\n", PROJECT_SOFTWARE_VERSION);
    Serial.printf("Enabling arduino\n");

    f_melbusController.Init();
    delay(100);
    f_melbusController.SetEnable(true);

    Serial.printf("Setting up control\n");
    f_mediaControl.SetSink(&f_bluetoothSink);
    f_mediaControl.SetDebug(true);

    Serial.printf("Starting bluetooth sink\n");
    InitA2DPSink();

    Serial.printf("Init Done!\n");
}

void loop() {
    f_mediaControl.Task();
    f_melbusController.Task();
}

void InitA2DPSink()
{
    i2s_pin_config_t my_pin_config = {
        .bck_io_num = DAC_BCLK_PIN,
        .ws_io_num = DAC_WSL_PIN,
        .data_out_num = DAC_DATA_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    f_bluetoothSink.set_pin_config(my_pin_config);

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
}