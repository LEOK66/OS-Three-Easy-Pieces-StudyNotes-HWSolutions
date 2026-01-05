#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *path = "/Users/leok/Desktop/OS-Three-Easy-Pieces-Homework-solution/5_ProcessAPI/example7.txt";
    int fork_id = fork();
    if (fork_id < 0) {
        perror("Fork failed");
        exit(1);
    } else if (fork_id == 0) {
        close(STDOUT_FILENO);
        // Child process
        int f_id = open(path, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        if (f_id < 0) {
            perror("Open failed");
            exit(1);
        }
        printf("This is a message from the child process (pid: %d)\n", getpid());
        fflush(stdout);
        close(f_id);
    } else {
        // Parent process
        printf("This is a message from the parent process (pid: %d)\n", getpid());
    }
    return 0;
}

// 父子线程的文件描述符是独立于彼此的，不会受影响。