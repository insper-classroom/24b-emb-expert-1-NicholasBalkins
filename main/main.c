#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define SPEAKER_PIN 28
#define BUTTON_PIN 15
#define MIC_PIN 26

volatile bool play_audio = false;

void button_callback(uint gpio, uint32_t events) {
    play_audio = !play_audio;
}

void setup_pwm() {
    gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SPEAKER_PIN);
    pwm_set_wrap(slice_num, 255);
    pwm_set_enabled(slice_num, true);
}

void setup_adc() {
    adc_init();
    adc_gpio_init(MIC_PIN);
    adc_select_input(0);
}

void play_microphone_audio() {
    uint16_t mic_value = adc_read();
    uint slice_num = pwm_gpio_to_slice_num(SPEAKER_PIN);
    pwm_set_gpio_level(SPEAKER_PIN, mic_value >> 4);
}

int main() {
    stdio_init_all();
    
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    setup_pwm();
    setup_adc();

    while (true) {
        if (play_audio) {
            play_microphone_audio();
        } else {
            pwm_set_gpio_level(SPEAKER_PIN, 0);
        }
        sleep_us(125);
    }
}
