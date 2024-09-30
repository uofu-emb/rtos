#pragma once
#include <FreeRTOS.h>
#include <semphr.h>

struct signal_data {
    int32_t input;
    int32_t output;
};

void signal_handle_calculation(SemaphoreHandle_t request,
                               SemaphoreHandle_t response,
                               struct signal_data *data);

BaseType_t signal_request_calculate(SemaphoreHandle_t request,
                                    SemaphoreHandle_t response,
                                    struct signal_data *data);
