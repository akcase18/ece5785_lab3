#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "lab3.h"
#include <FreeRTOS.h>
#include <semphr.h>

void setUp(void) {}

void tearDown(void) {}

/**
 * Should increment the counter every time 
 */
void test_side_thread()
{
    int counter = 0;
    SemaphoreHandle_t semaphore;
    TEST_ASSERT_TRUE(side_thread_logic(counter, semaphore) == 1);
}

void test_side_thread_locked()
{

}

void test_main_thread()
{
    int counter = 0;
    SemaphoreHandle_t semaphore;
    TEST_ASSERT_TRUE(main_thread_logic(counter, semaphore) == 1);
}

void test_main_thread_locked()
{

}

int main (void)
{
    stdio_init_all();
    // sleep_ms(5000); // Give time for TTY to attach.
    // printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_side_thread);
    RUN_TEST(test_side_thread_locked);
    RUN_TEST(test_main_thread);
    RUN_TEST(test_main_thread_locked);
    // sleep_ms(5000);
    return UNITY_END();
}
