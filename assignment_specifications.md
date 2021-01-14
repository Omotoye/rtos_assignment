
# Task

1. Design an application with 3 threads, whose periods are 300ms, 500ms, and 800ms.
1. The threads shall just _"waste time"_, as we did in the exercise with threads.
1. Design a simple driver with only __open__, __close__, __write__ system calls.
1. During its execution, every tasks 

    * (I). opens the special file associated with the driver;
    * (II) writes to the driver its own identifier plus open square brackets (i.e., [1, [2, or [3)
    * (III)close the special files
    * (IV) performs operations (i.e., wasting time)
    * (V)  performs (i)(ii) and (iii) again in order to write to the driver its own identifier, but with closed square brackets (i.e., 1], 2], or 3]).

1. The write system call simply writes on the kernel log the string that was received from the thread. 
A typical output of the system, by reading the kernel log, may be the following [11][2[11]2][3[11]3]  . 
This clearly shows that some threads can be preempted by other threads (if this does not happen, try to 
increase the computational time of longer tasks).

1. Finally, modify the code of all tasks in order to use __semaphores__. Every thread now protects all 
its operations (i) to (v) with a semaphore, which basically prevents other tasks from preempting it. 
Specifically, use semaphores with __priority ceiling__.  
