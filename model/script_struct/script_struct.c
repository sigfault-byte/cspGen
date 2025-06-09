

#include "script_struct.h"
#include <stdlib.h>
#include <stdbool.h>

ScriptMeta* create_script_meta() {
    ScriptMeta* meta = (ScriptMeta*) malloc(sizeof(ScriptMeta));
    if (!meta) return NULL;

    meta->open_tag_pos = -1;
    meta->open_tag_end_pos = -1;
    meta->close_tag_open_pos = -1;
    meta->close_tag_end_pos = -1;
    meta->size = -1;

    meta->has_nonce = false;
    meta->is_inline = false;

    meta->is_data_script = false;
    
    meta->has_src = false;
    meta->is_module = false;
    meta->is_data_uri = false;
    meta->is_protocol_based = false;
    meta->is_relative = false;
    meta->is_external = false;
    meta->has_crossorigin = false;

    //Span
    meta->src.start = -1;
    meta->src.end = -1;
    meta->src.length = -1;
    meta->src.has_sri = false;
    meta->src.sri_start = -1;
    meta->src.sri_end = -1;
    meta->src.sri_length = -1;
    
    meta->context = INLINE;
    meta->origin = IN_BODY;

    return meta;
}

//not used .. ?
void free_script_meta(ScriptMeta* meta) {
    if (meta) free(meta);
}