#ifndef HTML_STRUCT_H
#define HTML_STRUCT_H

#include "../script_struct/script_struct.h"

#define MAX_SCRIPT_CANDIDATES 1024

typedef struct {
    char* buffer;
    int length;

    // tag positions if found =D
    int html_open;
    int html_close;

    int head_open;
    int head_close;

    int body_open;
    int body_close;


    // Script tag prefilter
    int script_starts[MAX_SCRIPT_CANDIDATES];
    int script_closes[MAX_SCRIPT_CANDIDATES];
    int script_open_count;
    int script_close_count;

    ScriptMeta* scripts[MAX_SCRIPTS];
    int script_meta_count;

    int csp_flag;

} HtmlDoc;

//main struct
HtmlDoc* create_html_doc(char* raw_html);


//free
void free_html_doc(HtmlDoc* doc);

// Extract positions of html/head/body tags and prefilter <script candidates
void extract_relevant_tags_and_prefilter(HtmlDoc* doc, const int* open_tags, int open_count);

#endif

// Add in header for script struct population
void populate_script_structs(HtmlDoc* doc);

//free the scriptstruct
void free_script_meta_array(ScriptMeta** array, int count);



#define CSP_FLAG_INLINE_SCRIPT     (1 << 0)  // 1
#define CSP_FLAG_UNSAFE_EVAL       (1 << 1)  // 2
#define CSP_FLAG_DATA_URI_SCRIPT   (1 << 2)  // 4
#define CSP_FLAG_NONCE_PRESENT     (1 << 3)  // 8
#define CSP_FLAG_STRONG_HASHES     (1 << 4)  // 16
#define CSP_FLAG_WEAK_HASHES       (1 << 5)  // 32
#define CSP_FLAG_EXTERNAL_SCRIPT   (1 << 6)  // 64