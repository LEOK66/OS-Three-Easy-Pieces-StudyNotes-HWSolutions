#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe create failed");
        exit(1);
    }

    int child1_id = fork();
    if (child1_id < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child1_id == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        printf("Hello from Child1!\n");
        printf("Another message from Child1.\n");
        exit(0);
    }

    int child2_id = fork();
    if (child2_id < 0) {
        perror("Fork failed");
        exit(1);
    } else if (child2_id == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        char buffer[256];
        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            printf("Child2 received: %s", buffer);
        }
        exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);
    
    printf("Parent: both children finished\n");
    
    return 0;
}

// 当父亲的写端不关闭时，子进程2会一直阻塞在读取操作上，等待更多的数据输入。
// 通过关闭父进程的写端，子进程2能够检测到EOF，从而正确地结束读取循环。