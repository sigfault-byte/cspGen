#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include "../model/html_struct/html_struct.h"
#include "../model/signature/byte_signature.h"

void extract_relevant_tags_and_prefilter(HtmlDoc* doc, const int* open_tags, int open_count) {
    if (!doc || !doc->buffer) return;

    bool found_html_open = false;
    bool found_html_close = false;
    bool found_head_open = false;
    bool found_head_close = false;
    bool found_body_open = false;
    bool found_body_close = false;

    // struct timeval tv_start, tv_end;
    // gettimeofday(&tv_start, NULL);

    clock_t start = clock();

    for (int i = 0; i < open_count; i++) {
        int pos = open_tags[i] + 1;
        // Skip whitespace 
        // TODO: maybe limit to +3 /4
        while (
            doc->buffer[pos] == SPACE ||
            doc->buffer[pos] == TAB ||
            doc->buffer[pos] == NEWLINE ||
            doc->buffer[pos] == CR
        ) {
            pos++;
        }

        if (pos + 4 >= doc->length) continue;

        unsigned char raw = doc->buffer[pos];
        unsigned char c = raw | 0x20;
        // Group all 'h' prefixed tags (html, head)
        if ((c == H) && (!found_html_open || !found_head_open)) {
            // Only check if not already found
            if (!found_html_open &&
                ((doc->buffer[pos+1] | 0x20) == T) &&
                ((doc->buffer[pos+2] | 0x20) == M) &&
                ((doc->buffer[pos+3] | 0x20) == L)) {
                doc->html_open = open_tags[i];
                found_html_open = true;
                continue;
            }
            if (!found_head_open &&
                ((doc->buffer[pos+1] | 0x20) == E) &&
                ((doc->buffer[pos+2] | 0x20) == A) &&
                ((doc->buffer[pos+3] | 0x20) == D)) {
                doc->head_open = open_tags[i];
                found_head_open = true;
                continue;
            }
        }

        if (raw == SLASH) {
            unsigned char next = doc->buffer[pos+1] | 0x20;
            if ((next == H) && ((!found_html_close) || (!found_head_close))) {
                if (!found_html_close &&
                    ((doc->buffer[pos+2] | 0x20) == T) &&
                    ((doc->buffer[pos+3] | 0x20) == M) &&
                    ((doc->buffer[pos+4] | 0x20) == L)) {
                    doc->html_close = open_tags[i];
                    found_html_close = true;
                    continue;
                }
                if (!found_head_close &&
                    ((doc->buffer[pos+2] | 0x20) == E) &&
                    ((doc->buffer[pos+3] | 0x20) == A) &&
                    ((doc->buffer[pos+4] | 0x20) == D)) {
                    doc->head_close = open_tags[i];
                    found_head_close = true;
                    continue;
                }
            } else if ((next == B) && !found_body_close)  {
                if (!found_body_close &&
                    ((doc->buffer[pos+2] | 0x20) == O) &&
                    ((doc->buffer[pos+3] | 0x20) == D) &&
                    ((doc->buffer[pos+4] | 0x20) == Y)) {
                    doc->body_close = open_tags[i];
                    found_body_close = true;
                    continue;
                }
            } else if (raw == SLASH && next == S) {
                if ((doc->buffer[pos+2] | 0x20) == C &&
                    (doc->buffer[pos+3] | 0x20) == R &&
                    (doc->buffer[pos+4] | 0x20) == I &&
                    (doc->buffer[pos+5] | 0x20) == P &&
                    (doc->buffer[pos+6] | 0x20) == T) {

                    int max = pos + 15;
                    for (int j = pos + 7; j < max && j < doc->length; j++) {
                        if (doc->buffer[j] == GT) {
                            if (doc->script_close_count < MAX_SCRIPT_CANDIDATES) {
                                doc->script_closes[doc->script_close_count++] = j;
                            } else {
                                fprintf(stderr, "[WARN] script closing overflow\n");
                            }
                            break;
                        }
                    }
                }
            }
        }

        if ((c == B) && !found_body_open) {
            if (!found_body_open &&
                ((doc->buffer[pos+1] | 0x20) == O) &&
                ((doc->buffer[pos+2] | 0x20) == D) &&
                ((doc->buffer[pos+3] | 0x20) == Y)) {
                doc->body_open = open_tags[i];
                found_body_open = true;
                continue;
            }
        }

        if (c  == S) {
            if ((doc->buffer[pos+1] | 0x20) == C &&
                (doc->buffer[pos+2] | 0x20) == R &&
                (doc->buffer[pos+3] | 0x20) == I &&
                (doc->buffer[pos+4] | 0x20) == P &&
                (doc->buffer[pos+5] | 0x20) == T) {
                
                if (doc->script_open_count < MAX_SCRIPT_CANDIDATES) {
                    doc->script_starts[doc->script_open_count++] = open_tags[i];
                }
            }
        }
    }
    clock_t end = clock();
    double elapsed_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;

    printf("<html> open: %d\n", doc->html_open);
    printf("<html> close: %d\n", doc->html_close);
    printf("<head> open: %d\n", doc->head_open);
    printf("<head> close: %d\n", doc->head_close);
    printf("<body> open: %d\n", doc->body_open);
    printf("<body> close: %d\n", doc->body_close);
    printf("<script found: %d\n", doc->script_open_count);
    printf("</script found: %d\n", doc->script_close_count);
    printf("%d bytes html body head found and pre triage Scan completed in %.3f ms\n", doc->length ,elapsed_ms);

    // gettimeofday(&tv_end, NULL);
    // long total_ms = (tv_end.tv_sec - tv_start.tv_sec) * 1000L + (tv_end.tv_usec - tv_start.tv_usec) / 1000L;
    // printf("Total execution time (real): %ld ms\n", total_ms);
}