#include <zephyr.h>
#include <stdio.h>
#include <sys/printk.h>
extern void sleep_main(void);
extern void kernel_main(void);
extern void rtc_main(void);
extern void interrupt_main(void);
extern void busy_main(void);

void main(void)
{
    printf("Starting...\n");
#ifdef SLEEP_ACTIVITY
    printf("Running sleep\n");
    sleep_main();
#endif

#ifdef BUSY_ACTIVITY
    printf("Running busy\n");
    busy_main();
#endif

#ifdef KERNEL_ACTIVITY
    printf("Running kernel timer\n");
    kernel_main();
#endif

#ifdef RTC_ACTIVITY
    printf("Running RTC\n");

    rtc_main();
#endif

#ifdef INTERRUPT_ACTIVITY
    printf("Running interrupts\n");
    interrupt_main();
#endif
}
