// script_attribute_extract.c
#include <string.h>
#include <stdio.h>

#include "scripts_attribute_extract.h"
#include "../model/signature/byte_signature.h"
#include "../model/signature/byte_sequence_signature.h"

void extract_script_attributes(HtmlDoc* doc) {
    for (int i = 0; i < doc->script_meta_count; i++) {
        ScriptMeta* s = doc->scripts[i];

        int start = s->open_tag_pos;
        int end = s->open_tag_end_pos;

        if (start < 0 || end < 0 || end <= start || end > doc->length) continue;
        
        const char* range = doc->buffer + start;
        size_t range_len = end - start;

        // Search for nonce=
        const char* nonce_match = memmem(range, range_len, NONCE_EQ, NONCE_EQ_LEN);
        if (nonce_match && (nonce_match[6] == QUOTE || nonce_match[6] == APOSTROPHE)) {
            s->has_nonce = true;
        } else if (nonce_match) {
            fprintf(stderr, "`nonce=` attribute not followed by a QUOTE or APOSTROPHE.\n");
            return;
        }

        // Search for src=
        const char* src_match = memmem(range, range_len, SRC_EQ, SRC_EQ_LEN);
        if (src_match && (src_match[4] == QUOTE || src_match[4] == APOSTROPHE)) {
            s->has_src = true;
            s->src.quote_type = src_match[4] == QUOTE ? QUOTE_DOUBLE : QUOTE_SINGLE;
            // store the position of the next byte after the quote
            s->src.start = (int)(src_match - doc->buffer + 5);

            // look for the end of the src value by looking for the correct quote
            const char* value_start = src_match + 5; // skip `src="` or `src='`
            size_t remaining = (doc->buffer + end) - value_start;
            char quote = (s->src.quote_type == QUOTE_DOUBLE) ? QUOTE : APOSTROPHE;

            const char* end_src_match = memchr(value_start, quote, remaining);
            if (end_src_match) {
                s->src.end = (int)(end_src_match - doc->buffer);
                s->src.length = s->src.end - s->src.start;
            } else {
                fprintf(stderr, "No matching closing quote found for `src=` attribute.\n");
                return;
            }
        } else if (src_match) {
            fprintf(stderr, "`src=` attribute not followed by a QUOTE or APOSTROPHE.\n");
            return;
        //if no src -> it s inline
        } else if (!src_match) {
            s->is_inline = true;
        }

        // Search for type=
        const char* type_match = memmem(range, range_len, TYPE_EQ, TYPE_EQ_LEN);
        if (type_match && (type_match[5] == QUOTE || type_match[5] == APOSTROPHE)) {
            const char* value_start = type_match + 6;
            //Eliminate data_script non executable not handle by csp
            if (memcmp(value_start, APPLICATION, APPLICATION_LEN) == 0) {
                s->is_data_script = true;
                continue;
            }

            //module
            if (memcmp(value_start, MODULE, MODULE_LEN) == 0) {
                s->is_module = true;
            }
            // Search for crossorigin (can be valueless)
            const char* cross_match = memmem(range, range_len, CROSSORIGIN, CROSSORIGIN_LEN);
            if (cross_match) {
                if (cross_match[+ 1] != EQUAL){
                    fprintf(stderr, "`crossorigin` attribute not followed by a `=`.\n");
                }
                s->has_crossorigin = true;  
            }
            } else if (type_match) {
                fprintf(stderr, "`type` attribute not followed by a QUOTE or APOSTROPHE.\n");
                return;
            }

        // Search for integrity=
        const char* sri_match = memmem(range, range_len, INTEGRITY_EQ, INTEGRITY_EQ_LEN);
        if (sri_match && (sri_match[10] == QUOTE || sri_match[10] == APOSTROPHE)) {
            s->src.has_sri = true;
            s->src.sri_start = (int)(sri_match - doc->buffer + 11);

            //Get SRI value
            const char* value_start = sri_match + 11; // skip `integrity="` or `integrity='`
            size_t remaining = (doc->buffer + end) - value_start;
            char quote = (s->src.quote_type == QUOTE_DOUBLE) ? QUOTE : APOSTROPHE;

            const char* end_src_match = memchr(value_start, quote, remaining);
            if (end_src_match) {
                s->src.sri_end = (int)(end_src_match - doc->buffer);
                s->src.sri_length = s->src.end - s->src.start;
            } else {
                fprintf(stderr, "No matching closing quote found for `src=` attribute.\n");
                return;
            }

            // Search for crossorigin (can be valueless)
            const char* cross_match = memmem(range, range_len, CROSSORIGIN, CROSSORIGIN_LEN);
            if (cross_match) {
                if (cross_match[+ 1] != EQUAL){
                    fprintf(stderr, "`crossorigin` attribute not followed by a `=`.\n");
                }
                s->has_crossorigin = true;  
            }
        } else if (sri_match) {
            fprintf(stderr, "`integrity` attribute not followed by a QUOTE or APOSTROPHE.\n");
            return;
        }
    }

}

void sort_script_origin(HtmlDoc* doc) {
    for (int i = 0; i < doc->script_meta_count; i++) {
        ScriptMeta* s = doc->scripts[i];
        
        // bail on data_script
        if (s->is_data_script) {continue;}

        // buffer for the "src" 
        const char* value = doc->buffer + s->src.start;
        size_t len = s->src.length;

        //http:// warning
        if (len >= HTTP_LEN && memcmp(value, HTTP, HTTP_LEN) == 0) {
            s->is_external = true;
            fprintf(stderr, "Script uses `http` protocol:\n, ");
            for (int j = s->src.start; j < s->src.end; j++) {
                putchar(doc->buffer[j]);
            }
           continue;
        }
        //https://
        if (len >= HTTPS_LEN && memcmp(value, HTTPS, HTTPS_LEN) == 0) {
           s->is_external = true;
           continue;
        }
        //data: 
        if (len >= DATA_URI_LEN && memcmp(value, DATA_URI, DATA_URI_LEN) == 0) {
           s->is_data_uri = true;
           continue;
        }
        // protocol relative `//`
        if (len >= PROT_REL_LEN && value[0] == SLASH && value[1] == SLASH ) {
            s->is_protocol_based = true;
            fprintf(stderr, "Script uses protocol relative: \n, ");
            for (int j = s->src.start; j < s->src.end; j++) {
                putchar(doc->buffer[j]);
            }
           continue;
        }
        //relative
        if (len > 0 && (
            value[0] == '/' || 
            value[0] == '.' || 
            value[0] == '_' || 
            value[0] == '@' || 
            value[0] == '~' || 
            value[0] == '+'
        )) {
            s->is_relative = true;
            continue;
        }
        //else ?

    }
}