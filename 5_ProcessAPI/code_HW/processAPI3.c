#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe create failed");
        exit(1);
    }
    int fork_id = fork();
    if (fork_id < 0) {
        perror("Fork failed");
        exit(1);
    } else if (fork_id == 0) {
        close(pipefd[0]);
        printf("Child: Hello (pid: %d)\n", getpid());

        write(pipefd[1], "x", 1);
        close(pipefd[1]); 
    } else {
        close(pipefd[1]);
        char buf;
        read(pipefd[0], &buf, 1);
        close(pipefd[0]);
        printf("Parent: goodbye (pid: %d)\n", getpid());
    }
    return 0;
}

/**
这里可以使用管道来确保parent process永远在child process之后打印"goodbye"。
管道提供了一种简单的进程间通信机制，允许一个进程向另一个进程发送数据。在这个例子中，
child process在打印"Hello"后向管道写入一个字节的数据，然后parent process从管道读取这个字节，
确保它在child process完成后才继续执行并打印"goodbye"。这样就实现了所需的顺序。
*/