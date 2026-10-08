#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "driver/gpio.h"

#define LED_PIN 2

volatile uint32_t led_value = 0;
static TimerHandle_t tmr_led;

static void led_run_toggle(TimerHandle_t tmr_led)
{
    led_value ^= 0x1;
    gpio_set_level(LED_PIN, led_value);
}

void app_main() 
{
    gpio_config_t out_config = {};

    // creo config basica de output
    out_config.mode = GPIO_MODE_OUTPUT;
    out_config.intr_type = GPIO_INTR_DISABLE;
    out_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    out_config.pull_up_en = GPIO_PULLUP_DISABLE;

    // mapeo mascara con pines de salida
    out_config.pin_bit_mask = (1ULL << LED_PIN);
    // configuro salidas
    gpio_config(&out_config);

    // creo timer periodico
    tmr_led = xTimerCreate(
        "TMR_LED",
        pdMS_TO_TICKS(500),
        pdTRUE,
        NULL,
        led_run_toggle);
    
    // start timer
    xTimerStart(tmr_led, 0);

    while(1);
}