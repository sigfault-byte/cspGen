#ifndef SCRIPT_STRUCT_H
#define SCRIPT_STRUCT_H

#include <stdbool.h>

#define MAX_SCRIPTS 1024

typedef enum {
    INLINE = 0,
    EXTERNAL = 1,
    RELATIVE = 2
} ScriptContext;

typedef enum {
    QUOTE_NONE = 0,
    QUOTE_DOUBLE,  // " AKA quote
    QUOTE_SINGLE   // ' AKA apostrophe
} QuoteType;

typedef struct {
    int start;
    int end;
    int length;

    QuoteType quote_type;

    bool has_sri;

    int sri_start;
    int sri_end;
    int sri_length;
} Span;


typedef enum {
    IN_HEAD = 0,
    IN_BODY = 1
} ScriptOrigin;

typedef struct {
    int open_tag_pos; // --> <script
    int open_tag_end_pos; // <script> <--
    int close_tag_open_pos; // -> </script
    int close_tag_end_pos; // </script> <--
    int size;

    bool has_nonce;
    bool is_inline;

    bool is_data_script;

    bool has_src;
    bool is_module;
    bool is_data_uri;
    bool is_protocol_based;
    bool is_relative;
    bool is_external;
    bool has_crossorigin;

    Span src;

    ScriptContext context;
    ScriptOrigin origin;


} ScriptMeta;

ScriptMeta* create_script_meta(void);

#endif