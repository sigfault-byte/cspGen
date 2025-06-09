#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/time.h>

#include "html_fetch.h"
#include "extractor/tag_extract.h"
#include "extractor/script_inline_is_unsafe.h"
#include "model/html_struct/html_struct.h"

int main(int argc, char* argv[]) {
    struct timeval tv_start, tv_after_fetch;
    gettimeofday(&tv_start, NULL);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        return 1;
    }

    char* html = fetch_html(argv[1]);
    gettimeofday(&tv_after_fetch, NULL);
    if (!html) {
        fprintf(stderr, "Failed to fetch HTML.\n");
        return 1;
    }

    double total_s_fetch = (tv_after_fetch.tv_sec - tv_start.tv_sec) + (tv_after_fetch.tv_usec - tv_start.tv_usec) / 1000000.0;
    printf("Fetch duration (real): %.3f s\n", total_s_fetch);

    int html_length = strlen(html);

    int* lt_positions;
    
    
    int lt_count;

    
    find_all_tags(html, html_length, &lt_positions, &lt_count);

    HtmlDoc* doc = create_html_doc(html);
    if (!doc) {
        fprintf(stderr, "Failed to create HTML doc.\n");
        free(html);
        return 1;
    }

    extract_relevant_tags_and_prefilter(doc, lt_positions, lt_count);

    populate_script_structs(doc);

    script_inline_is_unsafe(doc);

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);
    long main_us = (tv_end.tv_sec - tv_after_fetch.tv_sec) * 1000000L + (tv_end.tv_usec - tv_after_fetch.tv_usec);
    printf("Main (after fetch): %ld µs\n", main_us);

    //FREE !
    free_script_meta_array(doc->scripts, doc->script_meta_count);
    free_html_doc(doc);

    double total_s = (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec) / 1000000.0;
    printf("Total execution time full pipeline: %.3f s\n", total_s);

    return 0;
}