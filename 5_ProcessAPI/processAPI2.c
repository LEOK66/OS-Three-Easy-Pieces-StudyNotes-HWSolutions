#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main() {
    const char *path = "/Users/leok/Desktop/OS-Three-Easy-Pieces-Homework-solution/5_ProcessAPI/example.txt";
    int f_id = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (f_id < 0) {
        perror("open fail");
        exit(1);
    }

    int fork_id = fork();
    if (fork_id < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (fork_id == 0) {
        const char *child_msg = "Message from Child Process\n";
        for (int i = 0; i < 5; i++) {
            write(f_id, child_msg, strlen(child_msg));
        }
        printf("Child Process wrote to file (pid: %d)\n", getpid());
    } else {
        const char *parent_msg = "Message from Parent Process\n";
        for (int i = 0; i < 5; i++) {
            write(f_id, parent_msg, strlen(parent_msg));
        }
        printf("Parent Process wrote to file (pid: %d)\n", getpid());
    }
    close(f_id);
    return 0;
}

// 这个程序说明了一个问题就是，多个进程可以同时写入同一个文件，但它们的写入操作是原子性的，不会相互干扰。