#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* data = (struct thread_data*) thread_param;

    // Sleep to wait before locking
    usleep(1000); // 1 millisecond

    // Lock the mutex
    if (pthread_mutex_lock(data->mutex) != 0) {
        data->thread_complete_success = false;
        pthread_exit(NULL);
    }

    // Sleep to wait after locking
    usleep(1000); // 1 millisecond

    // Unlock the mutex
    if (pthread_mutex_unlock(data->mutex) != 0) {
        data->thread_complete_success = false;
        pthread_exit(NULL);
    }

    // Set thread complete success
    data->thread_complete_success = true;

    // Return thread data
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    // Allocate memory for thread data
    struct thread_data *data = (struct thread_data *)malloc(sizeof(struct thread_data));
    if (data == NULL) {
        ERROR_LOG("Failed to allocate memory for thread data");
        return false;
    }

    // Fill thread data
    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;

    // Create thread
    if (pthread_create(thread, NULL, threadfunc, (void *)data) != 0) {
        ERROR_LOG("Failed to create thread");
        free(data); // Free allocated memory
        return false;
    }
    return true;
}

