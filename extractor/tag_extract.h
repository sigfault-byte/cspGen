#ifndef TAG_EXTRACT_H
#define TAG_EXTRACT_H

void find_all_tags(const char* html,
                    int html_length,
                    int** open_tags,
                    int* open_count);

#endif