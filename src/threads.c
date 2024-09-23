#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/cyw43_arch.h>
#include "lab3.h"

#define MAIN_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define SIDE_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

SemaphoreHandle_t semaphore;

int counter;
int on;

void side_thread(void *params)
{
    while (1)
    {
        vTaskDelay(100); // Sleep for 100ms
        counter = side_thread_logic(counter, semaphore);
    }
}

void main_thread(void *params)
{
    while (1)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on); // Flip the on-board LED from on to off, or from off to on
        vTaskDelay(100);                                // Sleep for 100ms
        counter = main_thread_logic(counter, semaphore);
        on = !on; // Change the state of "on"
    }
}

int main(void)
{
    stdio_init_all();                          // Initialize all GPIO
    hard_assert(cyw43_arch_init() == PICO_OK); // Ensure that on-board LED hardware is ok
    on = false;
    counter = 0;
    sleep_ms(5000);
    TaskHandle_t main, side;
    semaphore = xSemaphoreCreateCounting(1, 1);                                                    // Create semaphore
    xTaskCreate(main_thread, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &main); // Creating the main task
    xTaskCreate(side_thread, "SideThread", SIDE_TASK_STACK_SIZE, NULL, SIDE_TASK_PRIORITY, &side); // Creating the secondary task
    vTaskStartScheduler();                                                                         // Start FreeRTOS
    return 0;
}
