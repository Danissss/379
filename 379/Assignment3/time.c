#include <stdio.h>
#include <time.h>

/*
 * The result should look something like
 * Fri 2008-08-22 15:21:59 WAST
 */

int main(void)
{
    time_t     now;
    struct tm *ts;
    char       buf[80];

    /* Get the current time */
    now = time(NULL);
    int current_time = (long)now;
    /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */

    // strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
    printf("%ld seconds since the epoch began\n", (long)now);
    printf("%d seconds since the epoch began\n", current_time);

    printf("%s", asctime(gmtime(&now)));

    puts(buf);

    return 0;
}