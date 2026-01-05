#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int x = 100;

    int f_id = fork();
    if (f_id < 0) {
        perror("Fork failed");
        exit(1);
    } else if (f_id == 0) {
        printf("Child Process: x = %d (pid: %d)\n", x, getpid());
        x += 50;
        printf("Child Process after modification: x = %d (pid: %d)\n", x, getpid());
    } else {
        wait(NULL);
        printf("Parent Process: x = %d (pid: %d)\n", x, getpid());
        x -= 30;
        printf("Parent Process after modification: x = %d (pid: %d)\n", x, getpid());
    }

    return 0;
}

// 这个程序的核心概念就是x对于父子进程是互相独立的空间，不会互相影响