
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "../model/signature/byte_signature.h"
#include "../model/html_struct/html_struct.h"
#include "../model/script_struct/script_struct.h"
#include "scripts_attribute_extract.h"


void populate_script_structs(HtmlDoc* doc) {

    clock_t start = clock();
    if (doc->script_open_count != doc->script_close_count) {
        fprintf(stderr, "[WARN] script open / close element missimatch, %d, %d\n", doc->script_open_count, doc->script_close_count);
        //TODO: Something as fallback ?
    }


    // Allocate memory for ScriptMeta pointers in doc->scripts
    for (int i = 0; i < doc->script_open_count; i++) {
        doc->scripts[i] = create_script_meta();
        if (!doc->scripts[i]) {
            fprintf(stderr, "[ERROR] Failed to create ScriptMeta at index %d\n", i);
            continue;
        }
    }
    //store number of scripts , should already be a match
    doc->script_meta_count = doc->script_open_count;

    for (int i = 0; i < doc->script_open_count; i++) {
        int start = doc->script_starts[i];

        // Find the corresponding '>' to mark the end of opening tag
        int end = -1;
        for (int j = start + 4; j < doc->length; j++) {
            if (doc->buffer[j] == GT) {
                end = j;
                break;
            }
        }

        doc->scripts[i]->open_tag_pos = start;
        doc->scripts[i]->open_tag_end_pos = end;
        doc->scripts[i]->origin = (start > doc->body_open) ? IN_BODY : IN_HEAD;
    }

    for (int i = 0; i < doc->script_close_count; i++) {
        int end = doc->script_closes[i];

        // Find the corresponding '<' to mark the beginning of closing tag
        int start = -1;
        for (int j = end - 4; j > 0; j--) {
            if (doc->buffer[j] == LT) {
                start = j;
                break;
            }
        }

        doc->scripts[i]->close_tag_open_pos = start;
        doc->scripts[i]->close_tag_end_pos = end;
        int size = doc->scripts[i]->close_tag_open_pos - doc->scripts[i]->open_tag_end_pos - 2; // -2 for the < and >
        printf("Script %d start %d, end %d, size: %d\n", i, doc->scripts[i]->open_tag_end_pos, doc->scripts[i]->close_tag_open_pos, size );
        if (size > 0) {
            doc->scripts[i]->size = (size_t) size;
        }
    }

    // attr like sr nonc eintegrity module cross origin
    extract_script_attributes(doc);
    // sort the src into the correct origin
    sort_script_origin(doc);



    // timer etc
    clock_t end = clock();
    double elapsed_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    printf("Script struct populated linked to their origin in %.3f ms\n", elapsed_ms);
    
    printf("Populated %d script tag(s):\n", doc->script_meta_count);
    
    int ext = 0, dataURI = 0, module = 0, inlinejs = 0, dataScript = 0, relative = 0, nonce = 0, sri = 0;

    for (int i = 0; i < doc->script_meta_count; i++)  {
        ScriptMeta* s = doc->scripts[i];
            if (s->is_external) {ext ++;}
            if (s->is_data_uri) {dataURI ++;}
            if (s->is_module) {module ++;}
            if (s->is_inline) {inlinejs ++;}
            if (s->is_data_script) {dataScript ++;}
            if (s->has_nonce) {nonce ++;}
            if (s->src.has_sri) {sri ++;}
    }

    printf("Populated %d script external\n", ext);
    printf("Populated %d script with sri value \n", sri);
    printf("Populated %d script relative\n", relative);
    printf("Populated %d script dataURI\n", dataURI);
    printf("Populated %d script inline\n", inlinejs);
    printf("Populated %d script with a nonce value\n", nonce);
    printf("Populated %d script datascript\n", dataScript);
    printf("Populated %d script with  type=module\n", module);
}

void free_script_meta_array(ScriptMeta** array, int count) {
    if (!array) return;
    for (int i = 0; i < count; i++) {
        if (array[i]) {
            free(array[i]);
        }
    }
    // scripts[] is statically allocated in HtmlDoc, do NOT free the array itself.
    // free(array); Not malloc'd !
}