#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep()

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    char command[256];
    // Construct the command string from all arguments after the program name
    // This handles commands with spaces, e.g., "ls -l /tmp"
    int i;
    int offset = 0;
    for (i = 1; i < argc; i++) {
        offset += sprintf(command + offset, "%s ", argv[i]);
    }
    // Remove the trailing space if any
    if (offset > 0) {
        command[offset - 1] = '\0';
    } else {
        fprintf(stderr, "Error: No command provided.\n");
        return 1;
    }

    while (1) {
        // Clear the screen (ANSI escape code)
        // On AIX, 'clear' command or similar might also work,
        // but direct ANSI escape codes are generally more portable for C.
        printf("\033[H\033[J"); // Clears screen and moves cursor to top-left

        // Execute the command
        printf("--- Every 2.0s: %s ---\n", command);
        fflush(stdout); // Ensure prompt is displayed before command output
        system(command);

        // Pause for 2 seconds
        sleep(2);
    }

    return 0;
}