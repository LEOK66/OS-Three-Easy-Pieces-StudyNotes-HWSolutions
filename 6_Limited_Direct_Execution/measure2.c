#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>

// 计算时间差（微秒）
long time_diff_usec(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) * 1000000 + 
           (end->tv_usec - start->tv_usec);
}

// 测量 context switch 开销
void measure_context_switch() {
    int pipe1[2], pipe2[2];  // 两个 pipes
    char byte = 'X';
    int iterations = 100000;
    
    printf("Measuring context switch cost...\n");
    printf("Iterations: %d\n", iterations);
    printf("Expected context switches: %d\n\n", iterations * 2);
    
    // 创建两个 pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        // ========== 子进程 ==========
        close(pipe1[1]);  // 关闭 pipe1 的写端
        close(pipe2[0]);  // 关闭 pipe2 的读端
        
        for (int i = 0; i < iterations; i++) {
            read(pipe1[0], &byte, 1);   // 从 pipe1 读（等待父进程）
            write(pipe2[1], &byte, 1);  // 写到 pipe2（通知父进程）
        }
        
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
        
    } else {
        // ========== 父进程 ==========
        close(pipe1[0]);  // 关闭 pipe1 的读端
        close(pipe2[1]);  // 关闭 pipe2 的写端
        
        struct timeval start, end;
        gettimeofday(&start, NULL);
        
        for (int i = 0; i < iterations; i++) {
            write(pipe1[1], &byte, 1);  // 写到 pipe1（通知子进程）
            read(pipe2[0], &byte, 1);   // 从 pipe2 读（等待子进程）
        }
        
        gettimeofday(&end, NULL);
        
        close(pipe1[1]);
        close(pipe2[0]);
        
        wait(NULL);  // 等待子进程结束
        
        long total_usec = time_diff_usec(&start, &end);
        
        // 每次迭代有 2 次 context switch（父→子，子→父）
        int total_switches = iterations * 2;
        double avg_usec = (double)total_usec / total_switches;
        
        printf("Results:\n");
        printf("========================================\n");
        printf("Total time: %ld microseconds\n", total_usec);
        printf("Total context switches: %d\n", total_switches);
        printf("Average context switch cost: %.3f microseconds\n", avg_usec);
        printf("Average context switch cost: %.0f nanoseconds\n", avg_usec * 1000);
        printf("\nNote: CPU affinity not set (not supported on macOS)\n");
    }
}

int main() {
    printf("========================================\n");
    printf("Context Switch Cost Measurement (macOS)\n");
    printf("========================================\n\n");
    
    measure_context_switch();
    
    return 0;
}