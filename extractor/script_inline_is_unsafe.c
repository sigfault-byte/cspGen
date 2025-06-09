#include <string.h>
#include <stdio.h>
#include <time.h>

#include "script_inline_is_unsafe.h"
#include "../model/html_struct/html_struct.h"
#include "../model/signature/byte_signature.h"
#include "../model/signature/byte_js_signature.h"
#include "../model/signature/byte_sequence_signature.h"
#include "../config/settings.h"

// Not needed,  performance is either al ready good, or need to look again at the clang flag
// __attribute__((hot)) inline
// void match_js_signature(...) { ... }
// thinking in portal !

void script_inline_is_unsafe(HtmlDoc* doc) {

    clock_t start = clock();
    for (int i = 0; i < doc->script_meta_count; i++) {
        if (doc->csp_flag & CSP_FLAG_UNSAFE_EVAL) { return; }
        ScriptMeta* s = doc->scripts[i];
        // double check
         if (!s) {fprintf(stderr, "[WARN] pointer bug, no data at %i", i); continue; }
        // skip "not inline junk
        if (s->is_external || s->size <= 0 || s->is_data_script || s->is_data_uri) { continue ;} 
        // buffer for the content
        const char* value = doc->buffer + s->open_tag_end_pos + 1; //+1 for >
        size_t len = s->size;
        
        
        const char* end = value + len -1; // -1 for <
        int parentCount[PARENTHESES_LIMIT] = {0}; // init array at 0
        int paren_index = 0;
        
        while ((value = memchr(value, PARENTHESIS, end - value)) != NULL) {
            int index = (int)(value - doc->buffer); // global index for buffer
            
            
            if (paren_index < PARENTHESES_LIMIT) {
                parentCount[paren_index++] = index;
            } else {
                fprintf(stderr, "[WARN] Too many '()', limit reached at %d\n", PARENTHESES_LIMIT);
                break;
            }

            value++; // move past the found (
        }

        printf("Found %d '(' characters in script %d\n", paren_index, i);

        // now loop on the "(" pos and look 1 byte behind it
        for (int j = 0; j < paren_index; j++) {

            int parent_pos = parentCount[j];

            // L N T => const in byte_signature representing their bytes
            unsigned char preceding_byte = doc->buffer[parent_pos - 1];
            if (preceding_byte != L && preceding_byte != N && preceding_byte != T) {
                continue; // skip if not ending in 'l', 'n', or 't'
            }

            //example : 
            //sizeof(js_signatures)      = 3 * 24 = 72
            // sizeof(js_signatures[0])   = 24
            // sig_count = 72 / 24 = 3
            // This doesnt work outside the scope or if used as a pointer !
            size_t sig_count = sizeof(js_signatures) / sizeof(js_signatures[0]);
            for (size_t k = 0; k < sig_count; k++) {
                size_t sig_len = js_signatures[k].length;
                if (parent_pos >= 0 && (size_t)parent_pos >= sig_len) {
                    const char* candidate_start = doc->buffer + parent_pos - sig_len + 1;
                    if (memcmp(candidate_start, js_signatures[k].pattern, sig_len) == 0) {
                        doc->csp_flag |= CSP_FLAG_UNSAFE_EVAL;
                        printf("[CSPGEN] Matched %s at offset %d\n", js_signatures[k].label, parent_pos);
                        break;
                    }
                }
            }

        }
    }
    clock_t end = clock();
    int sum = 0;
    for (int i = 0; i < doc->script_meta_count; i++) {
        ScriptMeta* s = doc->scripts[i];
        if (s) sum += s->size;
    }
 
    double elapsed_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    printf("Finished scanning %d B of JS slop in %.3f ms\n", sum, elapsed_ms);

}
