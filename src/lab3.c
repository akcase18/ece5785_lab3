#include "lab3.h"

int side_thread_logic(int *counter, SemaphoreHandle_t semaphore)
{
    if (xSemaphoreTake(semaphore, 250) == pdFALSE)
    {
        return pdFALSE;
    }
    *counter = *counter + 1;                                       // Adding counter + 1 to counter
    printf("hello world from %s! Count %d\n", "thread", *counter); // Print out thread and the counter value
    xSemaphoreGive(semaphore);
    return pdTRUE;
}

int main_thread_logic(int *counter, SemaphoreHandle_t semaphore)
{
    if (xSemaphoreTake(semaphore, 250) == pdFALSE);
    {
        return pdFALSE;
    }
    printf("hello world from %s! Count %d\n", "main", (*counter)++); // Print out main and increment and print out counter
    SemaphoreGive(semaphore);
    return pdTRUE;
}

void deadlock(void *args)
{
    struct dl_args *dlargs = (struct dl_args *)args;

    TickType_t semaphore_delay = 250;

    dlargs->counter++;
    xSemaphoreTake(dlargs->semaphore1, semaphore_delay);
    {
        dlargs->counter++;
        vTaskDelay(100);
        xSemaphoreTake(dlargs->semaphore2, semaphore_delay);
        {
            dlargs->counter++;
        }
        xSemaphoreGive(dlargs->semaphore2);
    }
    xSemaphoreGive(dlargs->semaphore1);
    vTaskSuspend(NULL);
}

int orphaned_lock(int *counter, SemaphoreHandle_t semaphore)
{

}

int unorphaned_lock(int *counter, SemaphoreHandle_t semaphore)
{

}