#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// GPIO pin definitions from my ESP32
#define GreenLED 16  
#define RedLED 17  
#define E_PIN 26   
#define SEL_PIN 27 

// Timing parameters (for CHAUX)
#define T_a 0.3  
#define T_b 0.8  
#define P_c 5  
#define T_d 3.0
#define T_s 0.05 
static bool en = false;
static bool alt_mode = false;
//checks if the enable button is on 
void IRAM_ATTR en_isr(void *arg) {
    en = !en;
}
//checks if the selection mode is on 
void IRAM_ATTR sel_isr(void *arg) {
    alt_mode = !alt_mode;
}


void app_main() {
    //configures the Red and the green LEDS as outputs
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL <<GreenLED) | (1ULL <<RedLED),
        .mode = GPIO_MODE_OUTPUT,
    };
    //configure the buttons as inputs 
    gpio_config(&io_conf);
    io_conf.pin_bit_mask= (1ULL <<E_PIN) | (1ULL <<SEL_PIN);
    io_conf.mode=GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
//interupt handlers
    gpio_isr_handler_add(E_PIN, en_isr,NULL);
    gpio_isr_handler_add(SEL_PIN,sel_isr, NULL);
    while (1) {
        if (en) {//if enabled run the Signal Loop
            int pulses = alt_mode ? (P_c - 3) : P_c; // Adjust pulse count for alternative mode
            for (int i=0;i<pulses;i++) {
                gpio_set_level(GreenLED, 1);
                vTaskDelay(pdMS_TO_TICKS(T_a* 1000 + (i * 50)));
                gpio_set_level(GreenLED, 0);
                vTaskDelay(pdMS_TO_TICKS(T_b * 1000));
            }
            //Final Delays 
            vTaskDelay(pdMS_TO_TICKS(T_d *1000));
            gpio_set_level(RedLED, 1);
            vTaskDelay(pdMS_TO_TICKS(T_s* 1000));
            gpio_set_level(RedLED, 0);
            //if off then signal doesnt run 
        } else {
            gpio_set_level(GreenLED, 0);
            gpio_set_level(RedLED, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks
    }
}
