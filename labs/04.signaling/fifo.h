#pragma once
#include <FreeRTOS.h>
#include <queue.h>

struct request_msg {
    int32_t input;
    int32_t output;
    uint32_t handled_by;
};

void fifo_worker_handler(QueueHandle_t requests, QueueHandle_t results, int id);
