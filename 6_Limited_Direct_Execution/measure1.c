#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
long time_diff_usec(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) * 1000000 + 
           (end->tv_usec - start->tv_usec);
}

void test_timer_precision() {
    struct timeval t1, t2;
    long diff;
    long min_diff = 1000000; // Start with a large value
    for (int i = 0; i < 1000; i++) {
        gettimeofday(&t1, NULL);
        gettimeofday(&t2, NULL);

        diff = time_diff_usec(&t1, &t2);

        if (diff > 0 && diff < min_diff) {
            min_diff = diff;
        }
    }
    
    printf("Timer precision: ~%ld microseconds\n", min_diff);
}


int main() {
    struct timeval start, end;
    time_t sec_diff;
    suseconds_t usec_diff;
    int iteration = 1000000;

    // Get the start time
    if (gettimeofday(&start, NULL) == -1) {
        perror("gettimeofday");
        return 1;
    }

    for (int i = 0; i < iteration; i++) {
        // Empty loop to measure time taken
        read(0, NULL, 0);
    }

    // Get the end time
    if (gettimeofday(&end, NULL) == -1) {
        perror("gettimeofday");
        return 1;
    }

    long total_usec = time_diff_usec(&start, &end);
    double avg_usec = (double)total_usec / iteration;

    test_timer_precision();

    printf("Total time: %ld microseconds\n", total_usec);
    printf("Average system call cost: %.3f microseconds\n", avg_usec);
    printf("Average system call cost: %.0f nanoseconds\n\n", avg_usec * 1000);
    return 0;
}