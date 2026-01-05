#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fork_id = fork();
    if (fork_id < 0) {
        perror("Fork failed");
        exit(1);
    } else if (fork_id == 0) {
        int wait_return = wait(NULL);
        printf("Child Process: Hello (pid: %d)\n", getpid());
        printf("Child Process: wait returned %d (pid: %d)\n", wait_return, getpid());
    } else {
        printf("Parent Process: Goodbye (pid: %d)\n", getpid());
    }
}

// wait 函数会return 子进程的pid，因此父进程可以通过wait的返回值获知哪个子进程结束了。
// 加入wait函数在子进程中调用是没有意义的，因为子进程没有子进程可以等待，wait会立即返回-1并设置errno为ECHILD.
// waitpid() 让父进程可以选择性地等待特定的子进程，而不是像 wait() 那样只能等待"任意一个"子进程。