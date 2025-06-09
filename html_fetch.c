#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 8192

char* fetch_html(const char* url) {
    char command[1024];
    snprintf(command, sizeof(command), "curl -s -w 'HTTPSTATUS:%%{http_code}' -o - \"%s\"", url);

    FILE* pipe = popen(command, "r");
    if (!pipe) return NULL;

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char* buffer = malloc(buffer_size);
    if (!buffer) return NULL;

    size_t total_read = 0;
    int c;
    while ((c = fgetc(pipe)) != EOF) {
        if (total_read + 1 >= buffer_size) {
            buffer_size *= 2;
            char* new_buf = realloc(buffer, buffer_size);
            if (!new_buf) {
                free(buffer);
                pclose(pipe);
                return NULL;
            }
            buffer = new_buf;
        }
        buffer[total_read++] = (char)c;
    }

    buffer[total_read] = '\0';
    char* status_marker = strstr(buffer, "HTTPSTATUS:");
    if (status_marker) {
        int status_code = atoi(status_marker + 11); // skip "HTTPSTATUS:"
        printf("HTTP status code: %d\n", status_code);
        *status_marker = '\0'; // Truncate HTML before returning
    }
    pclose(pipe);
    return buffer;
}