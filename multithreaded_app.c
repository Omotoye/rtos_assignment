#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define THREAD_NUM 3
pthread_mutex_t task_mutex;
pthread_mutexattr_t task_mutex_attr;

// Type definition for the structure of the parameters for each of the threads
typedef struct
{
    int period;
    char write_message_1[3], write_message_2[3];
} thread_param;

// Task performed by each of the threads
void *thread_task(void *args)
{
    int fd;
    pthread_mutex_lock(&task_mutex);
    thread_param param = *(thread_param *)args;
    if ((fd = open("/dev/my_device", O_WRONLY)) < 0)
        perror("Open Failed");
    if ((write(fd, param.write_message_1, strlen(param.write_message_1) + 1)) < 0)
        perror("Write has failed");
    close(fd);
    pthread_mutex_unlock(&task_mutex);
    usleep(param.period);
    pthread_mutex_lock(&task_mutex);
    if((fd = open("/dev/my_device", O_WRONLY)) < 0)
        perror("Open Failed");
    if ((write(fd, param.write_message_2, strlen(param.write_message_2) + 1)) < 0)
        perror("Write Failed");
    close(fd);
    pthread_mutex_unlock(&task_mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t thread[THREAD_NUM];

    // Creating Mutex with priority ceiling attribute
    pthread_mutex_init(&task_mutex, &task_mutex_attr);
    pthread_mutexattr_init(&task_mutex_attr);
    pthread_mutexattr_setprotocol(&task_mutex_attr, PTHREAD_PRIO_PROTECT);
    pthread_mutexattr_setprioceiling(&task_mutex_attr, 10);

    // Creating the struct for each thread argument
    thread_param thread_1, thread_2, thread_3;
    thread_1.period = 300;
    thread_2.period = 500;
    thread_3.period = 800;
    strcpy(thread_1.write_message_1, "[1");
    strcpy(thread_1.write_message_2, "1]");
    strcpy(thread_2.write_message_1, "[2");
    strcpy(thread_2.write_message_2, "2]");
    strcpy(thread_3.write_message_1, "[3");
    strcpy(thread_3.write_message_2, "3]");

    thread_param thread_param_array[3];
    thread_param_array[0] = thread_1;
    thread_param_array[1] = thread_2;
    thread_param_array[2] = thread_3;

    // Creating the threads and passing the thread parameter as an argument
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&thread[i], NULL, &thread_task, &thread_param_array[i]) != 0)
        {
            perror("Failed to create thread");
        }
    }

    // Joining all the threads
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    printf("Finished Successfully\nCheck the Kernel Log the see the output\n");

    // Destroying the mutex object 
    pthread_mutexattr_destroy(&task_mutex_attr);
    pthread_mutex_destroy(&task_mutex);
    return 0;
}