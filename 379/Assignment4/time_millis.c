#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

long            ms_global; // Milliseconds
time_t          s_global;  // Seconds
int             start_time;


void print_current_time_with_ms (void)
{
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    printf("Current time: %"PRIdMAX".%03ld seconds since the Epoch\n",
           (intmax_t)s, ms);
    if (ms > 999) {
        s++;
        ms = 0;
    }

    printf("Current time: %"PRIdMAX".%03ld seconds since the Epoch\n",
           (intmax_t)s, ms);

    // struct timeval  tv;
    // gettimeofday(&tv, NULL);
    // double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ; // convert tv_sec & tv_usec to millisecond
    // printf("time_in_mill: %d",time_in_mill);
}

void get_time_gap(){
    struct timeval now;
    gettimeofday(&now, NULL);
    int time_now_sec = (long) now.tv_sec;
    int time_now_nanosec = (long) now.tv_usec;
    
    if (time_nano < 0){
        time - 1;
        time_nano = time_nano + 100000;
        int time_mil_sec = round(time_nano /1.0e3)
        int final_time = time*1000 + time_mil_sec;
    }
    else{
        int time_mil_sec = round(time_mil_sec /1.0e3)
        int final_time = time*1000 + time_mil_sec;
    }
}
int main(){
    // time_t program_start;
    // program_start = time(NULL);
	// int time_start_program = (long)program_start;
    // printf("time_start_program: %d\n",time_start_program);
    // usleep(1000);
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    s_global  = spec.tv_sec;
    ms_global = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    int time_global = (long)s_global;
    printf("time_global: %d\n",time_global);
    print_current_time_with_ms();
    get_time_today();
}
// int time = ((long) now.tv_sec - (long) now.tv_sec);
// int time_nano = ((long) now.tv_usec - (long) now.tv_usec);
if time_nano < 0:
    time - 1;
    time_nano = time_nano + 100000;
    int time_mil_sec = round(time_nano /1.0e3)
    int final_time = time*1000 + time_mil_sec;
else:
    int time_mil_sec = round(time_mil_sec /1.0e3)
    int final_time = time*1000 + time_mil_sec;
// Xuans-MacBook-Pro:Assignment4 xuan$ ./time_millis 
// time_global: 1543358624
// Current time: 1543358624.478 seconds since the Epoch
// Current time: 1543358624.478 seconds since the Epoch
// time_now_in_sec: 1543358624
// time_now_in_: 478298
// Xuans-MacBook-Pro:Assignment4 xuan$ ./time_millis 
// time_global: 1543358647
// Current time: 1543358647.190 seconds since the Epoch
// Current time: 1543358647.190 seconds since the Epoch
// time_now_in_sec: 1543358647
// time_now_in_: 190043
// Xuans-MacBook-Pro:Assignment4 xuan$ 