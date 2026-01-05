#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("Before fork\n");
    
    int fork_id = fork();
    if (fork_id < 0) {
        perror("Fork failed");
        exit(1);
    } else if (fork_id == 0) {
        printf("Child process executing ls...\n");
        execl("/bin/ls", "ls", "-l", "-a", NULL);
        perror("execl failed");
        exit(1);
    } else {
        wait(NULL);
        printf("Parent process: child finished\n");
    }
    
    return 0;
}