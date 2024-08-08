#include "fifo.h"
#include <zephyr.h>
#include <stdio.h>
#include <random/rand32.h>

void fifo_worker_handler(struct k_msgq *requests, struct k_msgq *results, int id)
{
    while (1) {
        struct request_msg data = {};
        k_msgq_get(requests, &data, K_FOREVER);
        data.output = data.input + 5;
        data.handled_by = id;
        k_msleep(sys_rand32_get() >> 24);
        k_msgq_put(results, &data, K_FOREVER);
    }
}
