#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>

#define THREAD_NUM 3
pthread_mutex_t task_mutex;
pthread_mutexattr_t task_mutex_attr;

typedef struct
{
    int period;
    char write_message_1[3], write_message_2[3];
} thread_param;

void *thread_task(void *args)
{
    int fd;
    pthread_mutex_lock(&task_mutex);
    thread_param param = *(thread_param *)args;
    fd = open("/dev/my_device", O_WRONLY);
    write(fd, param.write_message_1, strlen(param.write_message_1) + 1);
    close(fd);
    pthread_mutex_unlock(&task_mutex);
    usleep(param.period);
    pthread_mutex_lock(&task_mutex);
    fd = open("/dev/my_device", O_WRONLY);
    write(fd, param.write_message_2, strlen(param.write_message_2) + 1);
    close(fd);
    pthread_mutex_unlock(&task_mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t thread[THREAD_NUM];
    pthread_mutex_init(&task_mutex, &task_mutex_attr);
    pthread_mutexattr_init(&task_mutex_attr);
    pthread_mutexattr_setprotocol(&task_mutex_attr, PTHREAD_PRIO_PROTECT);
    pthread_mutexattr_setprioceiling(&task_mutex_attr, 10);

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

    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_create(&thread[i], NULL, &thread_task, &thread_param_array[i]) != 0)
        {
            perror("Failed to create thread");
        }
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
        {
            perror("Failed to join thread");
        }
    }
    printf("Finished Successfully\n");
    pthread_mutexattr_destroy(&task_mutex_attr);
    pthread_mutex_destroy(&task_mutex);
    return 0;
}

/*

Task

(1) Design an application with 3 threads, whose periods are 300ms, 500ms, and 800ms.

(2) The threads shall just "waste time", as we did in the exercise with threads.

(3) Design a simple driver with only open, close, write system calls.

(4) During its execution, every tasks 

	(i) opens the special file associated with the driver;

	(ii ) writes to the driver its own identifier plus open square brackets (i.e., [1, [2, or [3)

	(iii) close the special files

	(iv) performs operations (i.e., wasting time)

	(v) performs (i)(ii) and (iii) again in order to write to the driver its own identifier, b
    ut with closed square brackets (i.e., 1], 2], or 3]).

(5) The write system call simply writes on the kernel log the string that was received from the thread. 
A typical output of the system, by reading the kernel log, may be the following [11][2[11]2][3[11]3]  . 
This clearly shows that some threads can be preempted by other threads (if this does not happen, try to 
increase the computational time of longer tasks).

(6) Finally, modify the code of all tasks in order to use semaphores. Every thread now protects all 
its operations (i) to (v) with a semaphore, which basically prevents other tasks from preempting it. 
Specifically, use semaphores with priority ceiling.  

*/