#include <zephyr.h>
#include <stdio.h>
#include <random/rand32.h>

#define STACKSIZE 2000
#define THREAD_COUNT 4

struct request_msg {
    int32_t input;
    int32_t output;
    uint32_t handled_by;
};
struct k_thread supervisor_thread;
K_THREAD_STACK_DEFINE(supervisor_stack, STACKSIZE);

struct k_thread worker_threads[THREAD_COUNT];
K_THREAD_STACK_ARRAY_DEFINE(worker_stacks, THREAD_COUNT, STACKSIZE);

void worker_handler(struct k_msgq *requests, struct k_msgq *results, int id)
{
    while (1) {
        struct request_msg data = {};
        k_msgq_get(requests, &data, K_FOREVER);
        data.output = data.input + 5;
        data.handled_by = id;
        /* printf("%d: Calculated result %d for %d\n", */
        /*        data.handled_by, data.input, data.output); */
        k_msleep(sys_rand32_get() >> 24);
        k_msgq_put(results, &data, K_FOREVER);
    }
}

K_MSGQ_DEFINE(request, sizeof(struct request_msg), 32, 4);
K_MSGQ_DEFINE(response, sizeof(struct request_msg), 32, 4);

int main(void)
{
    for (int t = 0; t < THREAD_COUNT; t++) {
        k_thread_create(&worker_threads[t],
                        worker_stacks[t],
                        STACKSIZE,
                        (k_thread_entry_t) worker_handler,
                        &request,
                        &response,
                        (void *) t,
                        K_PRIO_COOP(7),
                        0,
                        K_NO_WAIT);
    }

    printf("Sending messages\n");
    for (int i = 0; i < 32; i++) {
        struct request_msg data = {};
        data.input = i;
        k_msgq_put(&request, &data, K_FOREVER);
    }

    for (int i = 0; i < 32; i++) {
        struct request_msg data = {};
        k_msgq_get(&response, &data, K_FOREVER);
        printf("Got result %d for %d, handled by thread %d\n",
               data.input, data.output, data.handled_by);
    }

    printf("Cleaning up\n");
    for (int t = 0; t < THREAD_COUNT; t++) {
        k_thread_abort(&worker_threads[t]);
    }
    printf("Done\n");
    return 0;
}
