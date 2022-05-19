#include <Arduino.h>
#include <project_pinconfig.h>
#include "BluetoothA2DPSink.h"
#include "melbus_controller.h"
#include "media_control.h"
#include "dev_print.h"
#include "media_interface.h"
#include "string.h"

bool DEV_PRINT_ENABLED = false;

#define PROJECT_SOFTWARE_VERSION "release-1.1"
#define BEGIN_WAIT_TIME_MS 2500
#define A2DP_SINK_NAME "My S80"

BluetoothA2DPSink f_bluetoothSink;
MediaControl f_mediaControl;
MelbusController f_melbusController(&f_mediaControl);

static void InitA2DPSink();

static void AVRCPMetaDataCallback(uint8_t count, const uint8_t* buffer);

void setup() {
    Serial.begin(230400);
    PRINTF1("Begin init bt receiver ver: %s\n", PROJECT_SOFTWARE_VERSION);

    PRINT("Starting bluetooth sink\n");
    InitA2DPSink();

    PRINT("Setting up control\n");
    f_mediaControl.SetSink(&f_bluetoothSink);
    f_mediaControl.SetDebug(true);

    PRINT("Enabling arduino\n");
    f_melbusController.Init();
    delay(BEGIN_WAIT_TIME_MS);
    f_melbusController.SetEnable(true);

    PRINT("Init Done!\n");
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

    f_bluetoothSink.start(A2DP_SINK_NAME);
    f_bluetoothSink.set_volume(100);
    f_bluetoothSink.set_avrc_metadata_callback(AVRCPMetaDataCallback);
    f_bluetoothSink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_ARTIST |
                                                        ESP_AVRC_MD_ATTR_TITLE |
                                                        ESP_AVRC_MD_ATTR_TRACK_NUM |
                                                        ESP_AVRC_MD_ATTR_NUM_TRACKS);
}

void AVRCPMetaDataCallback(uint8_t mask, const uint8_t* buffer)
{
    String bufferString = String((const char*)buffer);

    switch (mask)
    {
    case ESP_AVRC_MD_ATTR_ARTIST:
        PRINT("AVRCP artist: ");
        break;

    case ESP_AVRC_MD_ATTR_TITLE:
        PRINT("AVRCP title: ");
        break;

    case ESP_AVRC_MD_ATTR_TRACK_NUM:
        {
            PRINT("AVRCP track: ");
            uint8_t track = bufferString.toInt();
            PRINTF1("track: %i\n", track);
            break;
        }

    case ESP_AVRC_MD_ATTR_NUM_TRACKS:
        {
            PRINT("AVRCP tracks total: ");
            uint8_t disc = bufferString.toInt();
            PRINTF1("disc: %i\n", disc);
            break;
        }
    
    default:
        break;
    }
    PRINTLN((const char*)buffer);
}