#include "lab3.h"

int side_thread_logic(int counter, SemaphoreHandle_t semaphore)
{
    xSemaphoreTake(semaphore, 0);
    {
        counter += counter + 1;                                       // Adding counter + 1 to counter
        printf("hello world from %s! Count %d\n", "thread", counter); // Print out thread and the counter value
    }
    xSemaphoreGive(semaphore);

    return counter;
}

int main_thread_logic(int counter, SemaphoreHandle_t semaphore)
{
    xSemaphoreTake(semaphore, 0);
    {
        printf("hello world from %s! Count %d\n", "main", counter++); // Print out main and increment and print out counter
    }
    xSemaphoreGive(semaphore);

    return counter;
}