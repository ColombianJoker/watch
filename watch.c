#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()
#include <string.h> // For strlen(), strcpy(), getenv()
#include <time.h>   // For time(), strftime(), localtime()

// Default values as constants
#define DEFAULT_SLEEP_INTERVAL 2
#define DEFAULT_DATE_FORMAT "%H:%M:%S "
#define DEFAULT_OPEN_SEP "[ "
#define DEFAULT_CLOSE_SEP " ]"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Syntax: %s <command>\n", argv[0]);
        fprintf(stderr, "Environment variables:\n");
        fprintf(stderr, "  WATCH_SLEEP       -- Number of seconds to wait between command and command execution. (Default: %d)\n", DEFAULT_SLEEP_INTERVAL);
        fprintf(stderr, "  WATCH_DATEFORMAT  -- strftime() valid date and time format string. (Default: \"%s\")\n", DEFAULT_DATE_FORMAT);
        fprintf(stderr, "  WATCH_OPEN_SEP    -- String delimiter to delimit command (open). (Default: \"%s\")\n", DEFAULT_OPEN_SEP);
        fprintf(stderr, "  WATCH_CLOSE_SEP   -- String delimiter to delimit command (close). (Default: \"%s\")\n", DEFAULT_CLOSE_SEP);
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
    int sleep_interval = DEFAULT_SLEEP_INTERVAL; // Default sleep interval
    char *watch_sleep_env = getenv("WATCH_SLEEP");
    if (watch_sleep_env != NULL) {
        int env_sleep = atoi(watch_sleep_env);
        if (env_sleep > 0) {
            sleep_interval = env_sleep;
        } else {
            fprintf(stderr, "Warning: Invalid WATCH_SLEEP value '%s'. Using default %d seconds.\n", watch_sleep_env, DEFAULT_SLEEP_INTERVAL);
        }
    }

    // Get date format from WATCH_DATEFORMAT environment variable
    const char *date_format = DEFAULT_DATE_FORMAT; // Default date format
    char *watch_dateformat_env = getenv("WATCH_DATEFORMAT");
    if (watch_dateformat_env != NULL) {
        date_format = watch_dateformat_env;
    }
    
    // Get command open marker separator from WATCH_OPEN_SEP
    const char *open_sep = DEFAULT_OPEN_SEP;
    char *watch_open_sep_env = getenv("WATCH_OPEN_SEP");
    if (watch_open_sep_env != NULL) {
      open_sep = watch_open_sep_env;
    }

    // Get command close marker separator from WATCH_CLOSE_SEP
    const char *close_sep = DEFAULT_CLOSE_SEP;
    char *watch_close_sep_env = getenv("WATCH_CLOSE_SEP");
    if (watch_close_sep_env != NULL) {
      close_sep = watch_close_sep_env;
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
        printf("%s--- Every %d s: %s%s%s\n", time_buffer, sleep_interval, open_sep, command, close_sep);
        fflush(stdout);

        // Execute the command
        system(command);

        // Pause for the specified interval
        sleep(sleep_interval);
    }

    return 0;
}