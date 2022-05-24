#ifndef ZIC_SERVER_ESP32_H_
#define ZIC_SERVER_ESP32_H_

// ESP32 dev module -> audio kit

#include <zic_wave_wavetable.h>
#include <wavetables/wavetable_Bank.h>

#include <driver/i2s.h>

// I2S
#define PIN_I2S_AUDIO_KIT_MCLK 0
#define PIN_I2S_AUDIO_KIT_BCK 27
#define PIN_I2S_AUDIO_KIT_WS 25
#define PIN_I2S_AUDIO_KIT_DATA_OUT 26
#define PIN_I2S_AUDIO_KIT_DATA_IN 35

const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number

Zic_Wave_Wavetable wave(&wavetable_Bank);

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 128,
    .use_apll = 0
};

i2s_pin_config_t pins = {
    .bck_io_num = PIN_I2S_AUDIO_KIT_BCK,
    .ws_io_num = PIN_I2S_AUDIO_KIT_WS,
    .data_out_num = PIN_I2S_AUDIO_KIT_DATA_OUT,
    .data_in_num = PIN_I2S_AUDIO_KIT_DATA_IN
};

void zicServerEsp32Init()
{
    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pins);
    i2s_set_sample_rates(i2s_num, SAMPLE_RATE);
    i2s_start(i2s_num);
    // if (ESP32AudioCodec.Codec_Id == Codec_ID_ES8388) {
    //     REG_WRITE(PIN_CTRL, 0xFFFFFFF0);
    //     PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    // }
}

void zicServerEsp32Loop()
{
    static union sampleTUNT {
        uint32_t sample;
        int16_t ch[2];
    } sampleDataU;

    for (int i = 0; i < 512; ++i) {
        sampleDataU.ch[0] = wave.next();
        sampleDataU.ch[1] = sampleDataU.ch[0];
        static size_t bytes_written = 0;
        i2s_write(i2s_num, (const char*)&sampleDataU.sample, 4, &bytes_written, portMAX_DELAY);
    }
}

#endif