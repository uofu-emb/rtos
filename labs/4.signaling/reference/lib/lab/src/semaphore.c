#include <stdio.h>
#include <zephyr.h>
#include "signaling.h"

void signal_handle_calculation(struct k_sem *request,
                               struct k_sem *response,
                               struct signal_data *data)
{
    printf("+ Waiting for request\n");
    k_sem_take(request, K_FOREVER);
    printf("+ Handling calculation\n");
    data->output = data->input + 5;
    k_sleep(K_MSEC(1));
    printf("+ Done with calculation\n");
    k_sem_give(response);
}

int signal_request_calculate(struct k_sem *request,
                             struct k_sem *response,
                             struct signal_data *data)
{
    printf("- Handoff to worker\n");
    k_sem_give(request);
    printf("- Waiting for results\n");
    int res = k_sem_take(response, K_MSEC(1000));
    printf("- Result ready\n");
    return res;
}
