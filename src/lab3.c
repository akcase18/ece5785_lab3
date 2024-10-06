#include "lab3.h"

int side_thread_logic(int *counter, SemaphoreHandle_t semaphore)
{
    if (xSemaphoreTake(semaphore, 100) == pdFALSE)
    {
        return pdFALSE;
    }
    {
        *counter = *counter + 1;                                       // Adding counter + 1 to counter
        printf("hello world from %s! Count %d\n", "thread", *counter); // Print out thread and the counter value
    }
    xSemaphoreGive(semaphore);
    return pdTRUE;
}

int main_thread_logic(int *counter, SemaphoreHandle_t semaphore)
{
    if (xSemaphoreTake(semaphore, 0) == pdFALSE);
    {
        return pdFALSE;
    }
    {
        printf("hello world from %s! Count %d\n", "main", (*counter)++); // Print out main and increment and print out counter
    }
    SemaphoreGive(semaphore);
    return pdTRUE;
}