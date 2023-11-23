/*
 * alarm_fork.c
 *
 * This version of alarm.c uses pthread_create to create a
 * separate thread to wait for each alarm to expire.
 */
#include <pthread.h>
#include "errors.h"

const char* timestamp()
{
    time_t rawtime = time(NULL);
    static char mbstr[100];
    strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", localtime(&rawtime));
    return mbstr;
}

typedef struct alarm_tag {
    int         seconds;
    char        message[64];
} alarm_t;

void *alarm_thread (void *arg)
{
    alarm_t *alarm = (alarm_t*)arg;
    int status;

    status = pthread_detach (pthread_self ());
    if (status != 0)
        err_abort (status, "Detach thread");
    sleep (alarm->seconds);
    printf ("\nalarm timer [(%d) %s] at %s\n", alarm->seconds, alarm->message, timestamp());
    free (alarm);
    return NULL;
}

int main (int argc, char *argv[])
{
    int status;
    char line[128];
    alarm_t *alarm;
    pthread_t thread;

    while (1) {
        printf ("Alarm> ");
        if (fgets (line, sizeof (line), stdin) == NULL) exit (0);
        if (strlen (line) <= 1) continue;
        alarm = (alarm_t*)malloc (sizeof (alarm_t));
        if (alarm == NULL)
            errno_abort ("Allocate alarm");

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf (line, "%d %64[^\n]", 
            &alarm->seconds, alarm->message) < 2) {
            fprintf (stderr, "Bad command\n");
            free (alarm);
        } else {
            printf ("start timer [(%d) %s] at %s\n", alarm->seconds, alarm->message, timestamp());
            status = pthread_create (
                &thread, NULL, alarm_thread, alarm);
            if (status != 0)
                err_abort (status, "Create alarm thread");
        }
    }
}
