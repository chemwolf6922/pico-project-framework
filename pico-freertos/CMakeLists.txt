set(PICO_FREERTOS_SOURCE $ENV{PICO_FREERTOS_PATH}/FreeRTOS-Kernel)

add_library(freertos
    ${PICO_FREERTOS_SOURCE}/event_groups.c
    ${PICO_FREERTOS_SOURCE}/list.c
    ${PICO_FREERTOS_SOURCE}/queue.c
    ${PICO_FREERTOS_SOURCE}/stream_buffer.c
    ${PICO_FREERTOS_SOURCE}/tasks.c
    ${PICO_FREERTOS_SOURCE}/timers.c
    ${PICO_FREERTOS_SOURCE}/portable/MemMang/heap_3.c
    ${PICO_FREERTOS_SOURCE}/portable/GCC/ARM_CM0/port.c
)

target_include_directories(freertos PUBLIC
    $ENV{PICO_FREERTOS_PATH}
    ${PICO_FREERTOS_SOURCE}
    ${PICO_FREERTOS_SOURCE}/include
    ${PICO_FREERTOS_SOURCE}/portable/GCC/ARM_CM0
)