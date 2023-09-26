#pragma once
#include <zephyr.h>

struct signal_data {
    int32_t input;
    int32_t output;
};

void signal_handle_calculation(struct k_sem *request,
                               struct k_sem *response,
                               struct signal_data *data);

int signal_request_calculate(struct k_sem *request,
                             struct k_sem *response,
                             struct signal_data *data);
