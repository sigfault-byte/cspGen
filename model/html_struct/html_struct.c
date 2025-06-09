#include <stdlib.h>
#include <string.h>
#include "html_struct.h"

HtmlDoc* create_html_doc(char* raw_html) {
    if (!raw_html) return NULL;

    HtmlDoc* doc = malloc(sizeof(HtmlDoc));
    if (!doc) return NULL;

    doc->length = strlen(raw_html);
    doc->buffer = raw_html;  // take ownership

    //
    doc->csp_flag = 0; // int for bitflag

    // Init all positions to -1 (not found)
    doc->html_open = -1;
    doc->html_close = -1;
    doc->head_open = -1;
    doc->head_close = -1;
    doc->body_open = -1;
    doc->body_close = -1;

    // Script tag prefilter
    doc->script_open_count = 0;
    doc->script_close_count = 0;
    doc->script_meta_count = 0;
    for (int i = 0; i < MAX_SCRIPT_CANDIDATES; i++) {
        doc->script_starts[i] = -1;
        doc->script_closes[i] = -1;
    }
    for (int i = 0; i < MAX_SCRIPTS; i++) {
        doc->scripts[i] = NULL;
    }


    return doc;
}

// freeeeee
void free_html_doc(HtmlDoc* doc) {
    if (!doc) return;
    free(doc->buffer);  
    free(doc);
}