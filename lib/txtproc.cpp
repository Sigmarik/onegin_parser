#include "txtproc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "util/dbg/debug.h"

int compare_lines(const void* str_a, const void* str_b) {
    return strcmp(*(const char**)str_a, *(const char**)str_b);
}

int compare_reverse_lines(const void* void_a, const void* void_b) {
    const char* str_a = *(const char**) void_a;
    const char* str_b = *(const char**) void_b;
    int id_a = strlen(str_a), id_b = strlen(str_b);
    while (--id_a >= 0 && --id_b >= 0) {
        if (str_a[id_a] > str_b[id_b]) return 1;
        if (str_a[id_a] < str_b[id_b]) return -1;
    }
    if (id_a > id_b) return 1;
    if (id_a < id_b) return -1;
    return 0;
}

void reverse(char* string, int* error_code) {
    _LOG_FAIL_CHECK_(string, "error", ERROR_REPORTS, return;, error_code, EFAULT);
    int length = strlen(string);
    for (int id = 0; id * 2 < length - 1; id++) {
        char mem = string[id];
        string[id] = string[length - id - 1];
        string[length - id - 1] = mem;
    }
}

int read_file(const char* file_name, char*** text, int* error_code) {
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);

    FILE* file = fopen(file_name, "r");
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, ENOENT);

    *text = (char**)calloc(FILE_MAX_LENGTH, sizeof(char*));
    for (int id = 0; id < FILE_MAX_LENGTH; id++) {
        (*text)[id] = (char*)calloc(LINE_MAX_LENGTH, sizeof(char));
    }

    for (int line_id = 0; line_id < FILE_MAX_LENGTH; ++line_id) {
        int eof_detected = 0;  //         '\0' at the end v
        for (int char_id = 0; char_id < LINE_MAX_LENGTH - 1; char_id++) {
            char curent = getc(file);
            if (curent == '\n') break;
            if (curent == EOF) {
                eof_detected = 1;
                break;
            }
            (*text)[line_id][char_id] = curent;
        }
        if (eof_detected) return line_id + 1;
    }

    if (error_code) *error_code = EFBIG;
    
    return FILE_MAX_LENGTH;
}

void write_file(const char* file_name, const char*const* text, int text_length, int* error_code) {
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text, "error", ERROR_REPORTS, return;, error_code, EFAULT);

    FILE* file = fopen(file_name, "w");
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return;, error_code, ENOENT);
    for (int line_id = 0; line_id < text_length; line_id++) {
        fprintf(file, "%s\n", text[line_id]);
    }
}