#include "txtproc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "util/dbg/debug.h"

int compare_lines(const void* str_a, const void* str_b) {
    return wcscmp(*(cstr*)str_a, *(cstr*)str_b);
}

int compare_reverse_lines(const void* void_a, const void* void_b) {
    cstr str_a = *(cstr*) void_a;
    cstr str_b = *(cstr*) void_b;
    int id_a = wcslen(str_a), id_b = wcslen(str_b);
    while (--id_a >= 0 && --id_b >= 0) {
        if (str_a[id_a] > str_b[id_b]) return 1;
        if (str_a[id_a] < str_b[id_b]) return -1;
    }
    if (id_a > id_b) return 1;
    if (id_a < id_b) return -1;
    return 0;
}

int read_file(const char* file_name, str* *text, str *buffer, int* error_code) {
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(buffer, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);

    FILE* file = fopen(file_name, "r");
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, ENOENT);

    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    int line_count = 1;

    *buffer = (str)calloc(file_size, sizeof(**buffer));
    for (int char_id = 0; char_id < file_size; char_id++) {
        //fread(*buffer + char_id, sizeof(**buffer), 1, file);
        (*buffer)[char_id] = fgetwc(file);
        if ((*buffer)[char_id] == '\n') line_count++;
    }

    *text = (str*)calloc(line_count, sizeof(**text));
    (*text)[0] = *buffer;
    int line_id = 0;
    for (int char_id = 0; char_id < file_size; char_id++) {
        if ((*buffer)[char_id] == (wchar_t)'\n') {
            (*buffer)[char_id] = 0;
            (*text)[++line_id] = (*buffer) + char_id + 1;
        }
    }

    return line_count;
}

void write_file(const char* file_name, const cstr* text, int text_length, int* error_code) {
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text, "error", ERROR_REPORTS, return;, error_code, EFAULT);

    FILE* file = fopen(file_name, "w");

    //! TODO: think if you can speed up writing with syscall (like write on linux)
    //! It will not optimize writing as, probably, compiler already transforms
    //! high level fprintf calls to low-level syscalls for exact system or even more
    //! optimized low-level counterpart.
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return;, error_code, ENOENT);
    for (int line_id = 0; line_id < text_length; line_id++) {
        int char_id = 0;
        while (text[line_id][char_id] != 0) {
            fputwc(text[line_id][char_id], file);
            char_id++;
        }
        fputwc('\n', file);
    }
}