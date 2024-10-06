#ifndef LAB3_H
#define LAB3_H

#include <FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>

struct dl_args {
    SemaphoreHandle_t semaphore1;
    SemaphoreHandle_t semaphore2;
    int counter;
    char id;
};

/**
 * Logic block inside of the side thread infinite loop. This funciton
 * will take in the value of the counter, increase the value of the 
 * counter, and then print out a statement. The entire function is gated
 * by a semaphore.
 * 
 * @param counter the global counter variable
 * @param semaphore SemaphoreHandle_t object that points to created semaphore
 * 
 * @return the new counter value
 */
int side_thread_logic(int *counter, SemaphoreHandle_t semaphore);

/**
 * Logic block inside of the main thread infinite loop. This function
 * will take in the value of the counter, print out a statement, and
 * then increment the counter. The entire function is gated by a 
 * semaphore
 * 
 * @param counter the global counter variable
 * @param semaphore SemaphoreHandle_t object that points to created semaphore
 * 
 * @return the incremented counter
 */
int main_thread_logic(int *counter, SemaphoreHandle_t semaphore);

void deadlock(void *args);
int orphaned_lock(int *counter, SemaphoreHandle_t semaphore);
int unorphaned_lock(int *counter, SemaphoreHandle_t semaphore);

#endif