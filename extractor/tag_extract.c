#include <stdio.h>
#include <string.h> // for memchr
#include <time.h>

#include "../model/signature/byte_signature.h"
#include "tag_extract.h"
#include "../config/settings.h"

// Find all '<' and '>' positions in html, store them in open_tags, close_tags arrays.

void find_all_tags(const char *html, int length, int **open_tags, int *open_count) {
    static int open_tag_buffer[TAG_LIMIT];
    *open_tags = open_tag_buffer;
    *open_count = 0;

    clock_t start = clock();

    const char* p = html;
    const char* end = html + length;
    while ((p = memchr(p, LT, end - p)) != NULL) {
        int index = (int)(p - html);
        if (*open_count >= TAG_LIMIT) {
            fprintf(stderr, "[WARN] Too many '<' tags, limit reached at %d\n", TAG_LIMIT);
            break;
        }
        (*open_tags)[(*open_count)++] = index;
        p++; // Move past current match
    }

    clock_t t_end = clock();
    double elapsed_ms = 1000.0 * (t_end - start) / CLOCKS_PER_SEC;

    printf("Found %d '<' bytes\n", *open_count);
    printf("Scan completed in %.3f ms\n", elapsed_ms);
}