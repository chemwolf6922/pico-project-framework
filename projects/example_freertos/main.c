#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "pico/stdlib.h"

static void example_task(void *ctx)
{
    for(;;)
    {
        printf("Hello\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main()
{

    stdio_init_all();

    printf("start\n");

    xTaskCreate(example_task, "example", 2048, NULL, 1, NULL);

    vTaskStartScheduler();

    for(;;)
        tight_loop_contents();

    return 0;
}