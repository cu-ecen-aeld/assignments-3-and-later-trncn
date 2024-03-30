#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    // Check if two arguments are provided
    if (argc != 3) {
        fprintf(stderr, "Error: Two arguments are required.\n");
        exit(EXIT_FAILURE);
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];

    // Open file for writing
    FILE *file = fopen(writefile, "w");
    if (file == NULL) {
        syslog(LOG_ERR, "Error opening file: %s", writefile);
        perror("Error");
        exit(EXIT_FAILURE);
    }

    // Write string to file
    fprintf(file, "%s", writestr);
    fclose(file);

    // Log the write operation
    syslog(LOG_DEBUG, "Writing \"%s\" to %s", writestr, writefile);

    closelog();
    return EXIT_SUCCESS;
}
