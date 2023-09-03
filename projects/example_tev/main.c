#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "pico/stdlib.h"
#include "tev.h"
#include "tev_irq_injector.h"
#include "hardware/gpio.h"

/** helper to filter interrupts, do not do this inside the event loop */
#define filter_irq(min_gap_ms) ({\
    static uint32_t last_timestamp_o1fd1v = 0;\
    uint32_t now_timestamp_o1fd1v = to_ms_since_boot(get_absolute_time());\
    int ret__o1fd1v = 0;\
    if((last_timestamp_o1fd1v != 0) && ((now_timestamp_o1fd1v - last_timestamp_o1fd1v) < (min_gap_ms)))\
    {\
        ret__o1fd1v = -1;\
    }\
    last_timestamp_o1fd1v = now_timestamp_o1fd1v;\
    ret__o1fd1v;\
})

#define INPUT_PIN (2)

static tev_handle_t tev = NULL;
static tev_irq_injector_handle_t injector = NULL;
static tev_irq_handle_t gpio_irq_handle = NULL;

/** the raw irq handler */
static void gpio_irq_handler(uint gpio, uint32_t event_musk)
{
    if((gpio == INPUT_PIN) && (event_musk & GPIO_IRQ_EDGE_FALL))
    {
        if(filter_irq(100)==0)
        {
            /** inject this irq into the event loop */
            tev_irq_injector_inject(injector,gpio_irq_handle);
        }
    }
}

/** irq handler in the event loop */
static void on_gpio_interrupt(void *ctx)
{
    /** update tp status */
    printf("interrupt!\n");
}

static void tev_task(void *ctx)
{

    tev = tev_create_ctx();
    injector = tev_irq_injector_new(tev, 10);

    /** init gpio irq */
    gpio_irq_handle = tev_irq_injector_set_irq_handler(injector,on_gpio_interrupt,NULL);
    gpio_init(INPUT_PIN);
    gpio_set_dir(INPUT_PIN,GPIO_IN);
    gpio_pull_up(INPUT_PIN);
    gpio_set_irq_enabled_with_callback(INPUT_PIN,GPIO_IRQ_EDGE_FALL,true,gpio_irq_handler);

    printf("Init done, enter ev\n");

    tev_main_loop(tev);

    tev_irq_injector_free(injector);
    injector = NULL;
    tev_free_ctx(tev);
    tev = NULL;
}

int main()
{

    stdio_init_all();

    printf("start\n");

    xTaskCreate(tev_task, "TEV", 2048, NULL, 1, NULL);

    vTaskStartScheduler();

    printf("exit\n");

    for(;;)
        tight_loop_contents();

    return 0;
}