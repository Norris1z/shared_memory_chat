#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define BUFFER_SIZE 200
#define USER_READY 1
#define USER_NOT_READY -1
#define FILLED 0

typedef struct buffer
{
    char data[BUFFER_SIZE];
    int user_status;
    int user1_pid;
    int user2_pid;
} Buffer;

Buffer *shared_memory_ptr;

void user1_sig_handler(int signal_id)
{
    if (signal_id == SIGUSR1)
    {
        printf("User 2: ");
        puts(shared_memory_ptr->data);
    }
}

int main()
{
    key_t key = ftok("shared_mem_chat", 99);
    int shared_memory_id = shmget(key, sizeof(Buffer), IPC_CREAT | 0666);
    shared_memory_ptr = (Buffer *)shmat(shared_memory_id, NULL, 0);
    shared_memory_ptr->user1_pid = getpid();
    shared_memory_ptr->user_status = USER_NOT_READY;

    signal(SIGUSR1, user1_sig_handler);

    while (1)
    {
        while (shared_memory_ptr->user_status != USER_READY)
            continue;
        sleep(1);
        printf("User 1: ");
        fgets(shared_memory_ptr->data, BUFFER_SIZE, stdin);
        shared_memory_ptr->user_status = FILLED;
        kill(shared_memory_ptr->user2_pid, SIGUSR2);
    }
    shmdt((void *)shared_memory_ptr);
    shmctl(shared_memory_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
