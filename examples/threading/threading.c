#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    if (thread_param != NULL) {
     
        struct thread_data *data = (struct thread_data*) thread_param;

        if (data->mutex != NULL) {
            usleep(data->obtain_wait_ms * 1000);
            int result_l = pthread_mutex_lock(data->mutex);
            usleep(data->release_wait_ms * 1000);
            int result_r = pthread_mutex_unlock(data->mutex);

            if (result_l == 0 && result_r == 0) {
                data->thread_complete_success = true;
            }
        }
        // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
        // hint: use a cast like the one below to obtain thread arguments from your parameter
        //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    }
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

    struct thread_data *data = (struct thread_data*) malloc(sizeof(struct thread_data));

    if (data != NULL) {
        data->obtain_wait_ms = wait_to_obtain_ms;
        data->release_wait_ms = wait_to_release_ms;
        data->mutex = mutex;
        data->thread_complete_success = false;

        int result = pthread_create(thread, NULL, threadfunc, data);
        if (result == 0) {
            return true;
        }
    }
     
    return false;
}

