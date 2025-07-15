#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()
#include <string.h> // For strlen(), strcpy(), getenv()
#include <time.h>   // For time(), strftime(), localtime()

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Syntax: %s <command>\n", argv[0]);
        fprintf(stderr, "Environment variables:\n");
        fprintf(stderr, "  WATCH_SLEEP       -- Number of seconds to wait between command and command execution.\n");
        fprintf(stderr, "  WATCH_DATEFORMAT  -- strftime() valid date and time format string.\n");
        return 1;
    }

    char command[256];
    // Construct the command string from all arguments after the program name
    int i;
    int offset = 0;
    for (i = 1; i < argc; i++) {
        offset += sprintf(command + offset, "%s ", argv[i]);
    }
    if (offset > 0) {
        command[offset - 1] = '\0'; // Remove trailing space
    } else {
        fprintf(stderr, "Error: No command provided.\n");
        return 1;
    }

    // Get sleep interval from WATCH_SLEEP environment variable
    int sleep_interval = 2; // Default sleep interval
    char *watch_sleep_env = getenv("WATCH_SLEEP");
    if (watch_sleep_env != NULL) {
        int env_sleep = atoi(watch_sleep_env);
        if (env_sleep > 0) {
            sleep_interval = env_sleep;
        } else {
            fprintf(stderr, "Warning: Invalid WATCH_SLEEP value '%s'. Using default 2 seconds.\n", watch_sleep_env);
        }
    }

    // Get date format from WATCH_DATEFORMAT environment variable
    const char *date_format = "%H:%M:%S"; // Default date format
    char *watch_dateformat_env = getenv("WATCH_DATEFORMAT");
    if (watch_dateformat_env != NULL) {
        date_format = watch_dateformat_env;
    }

    while (1) {
        // Clear the screen (ANSI escape code)
        printf("\033[H\033[J"); // Clears screen and moves cursor to top-left

        // Get current date and time
        time_t rawtime;
        struct tm *info;
        char time_buffer[80];

        time(&rawtime);
        info = localtime(&rawtime);
        strftime(time_buffer, sizeof(time_buffer), date_format, info);

        // Display header with date, time, interval, and command
        printf("%s --- Every %d s: [ %s ]\n", time_buffer, sleep_interval, command);
        fflush(stdout);

        // Execute the command
        system(command);

        // Pause for the specified interval
        sleep(sleep_interval);
    }

    return 0;
}