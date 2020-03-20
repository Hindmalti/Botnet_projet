#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
//#include <libnetwork.h>
#include <libshm.h>

int main()
{
    char parent_message[] = "hello";  // parent process will write this message
    char child_message[] = "goodbye"; // child process will then write this one

    void *shmem = create_shared_memory(128);

    memcpy(shmem, parent_message, sizeof(parent_message));

    int pid = fork();

    if (pid == 0)
    {
        printf("Child read: %s\n", shmem);
        memcpy(shmem, child_message, sizeof(child_message));
        printf("Child wrote: %s\n", shmem);
    }
    else
    {
        printf("Parent read: %s\n", shmem);
        sleep(1);
        printf("After 1s, parent read: %s\n", shmem);
    }
}