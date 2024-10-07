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
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    TEST_ASSERT_TRUE(side_thread_logic(counter, semaphore) == pdTRUE);
    TEST_ASSERT_TRUE(counter == 1);
}

void test_side_thread_locked()
{
    int counter = 0;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    xSemaphoreTake(semaphore, 0);
    TEST_ASSERT_FALSE(side_thread_logic(counter, semaphore) == pdTRUE);
    TEST_ASSERT_TRUE(counter == 0);
}

void test_main_thread()
{
    int counter = 0;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    TEST_ASSERT_TRUE(main_thread_logic(counter, semaphore) == pdTRUE);
    TEST_ASSERT_TRUE(counter == 1);
}

void test_main_thread_locked()
{
    int counter = 0;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);
    xSemaphoreTake(semaphore, 0);
    TEST_ASSERT_FALSE(main_thread_logic(counter, semaphore) == pdTRUE);
    TEST_ASSERT_TRUE(counter == 0);
}

void test_deadlock()
{
    TaskHandle_t task1;
    TaskHandle_t task2;
    SemaphoreHandle_t semaphore1 = xSemaphoreCreateCounting(1, 1);
    SemaphoreHandle_t semaphore2 = xSemaphoreCreateCounting(1, 1);

    struct dl_args dlargs1 = {semaphore1, semaphore2, 0, 'thread 1'};
    struct dl_args dlargs2 = {semaphore2, semaphore1, 2, 'thread 2'};

    BaseType_t status1 = xTaskCreate(deadlock, "Task 1", configMINIMAL_STACK_SIZE, (void *)&dlargs1, 4, &task1);
    BaseType_t status2 = xTaskCreate(deadlock, "Task 2", configMINIMAL_STACK_SIZE, (void *)&dlargs2, 4, &task2);

    vTaskDelay(1000);

    TEST_ASSERT_TRUE(uxSemaphoreGetCount(semaphore1) == 0);
    TEST_ASSERT_TRUE(dlargs1.counter == 2);

    TEST_ASSERT_TRUE(uxSemaphoreGetCount(semaphore2) == 0);
    TEST_ASSERT_TRUE(dlargs2.counter == 4);

    vTaskDelete(task1);
    vTaskDelete(task2);
}

void test_orphaned_lock()
{
    int counter = 1;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    int result = orphaned_lock(&counter, semaphore);
    TEST_ASSERT_TRUE(counter == 2);
    TEST_ASSERT_TRUE(result == pdTRUE);
    TEST_ASSERT_TRUE(uxSemaphoreGetCount(semaphore) == 1);

    result = orphaned_lock(&counter, semaphore);
    TEST_ASSERT_TRUE(counter == 3);
    TEST_ASSERT_TRUE(result == pdFALSE);
    TEST_ASSERT_TRUE(uxSemaphoreGetCount(semaphore) == 0);

    result = orphaned_lock(&counter, semaphore);
    TEST_ASSERT_TRUE(counter == 3);
    TEST_ASSERT_TRUE(result == pdFALSE);
    TEST_ASSERT_TRUE(uxSemaphoreGetCount(semaphore) == 0);
}

void test_unorphaned_lock()
{
    int counter = 1;
    SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 1);

    int result;
    result = unorphaned_lock(&counter, semaphore);
    TEST_ASSERT_TRUE(counter == 2);
    TEST_ASSERT_TRUE(result == pdTRUE);
    TEST_ASSERT_TRUE(uxSemaphoreGetCount(semaphore) == 1);

    result = unorphaned_lock(&counter, semaphore);
    TEST_ASSERT_TRUE(counter == 3);
    TEST_ASSERT_TRUE(result == pdTRUE);
    TEST_ASSERT_TRUE(uxSemaphoreGetCount(semaphore) == 1);
}

void runner_thread(__unused void *args)
{
    while (1)
    {
        UNITY_BEGIN();
        RUN_TEST(test_side_thread);
        RUN_TEST(test_side_thread_locked);
        RUN_TEST(test_main_thread);
        RUN_TEST(test_main_thread_locked);
        RUN_TEST(test_deadlock);
        RUN_TEST(test_orphaned_lock);
        RUN_TEST(test_unorphaned_lock);
        // sleep_ms(5000);
        return UNITY_END();
    }
}

int main(void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    xTaskCreate(runner_thread, "TestRunner", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    vTaskStartScheduler();
    return 0;
}
