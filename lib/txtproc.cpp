#include "txtproc.h"

#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <cwctype>
#include <errno.h>

#include "util/dbg/debug.h"

/**
 * @brief Check if character shold not be skipped during sorting.
 * 
 * @param character symbol to check
 * @return bool 1 if character must be sorted and 0 if it isn't
 */
static inline int iswsortable(const wchar_t character) {
    return iswalpha(character) || iswdigit(character);
}

/**
 * @brief Get the file length object.
 * 
 */
static size_t get_file_length(FILE* file) {
    struct stat buffer;
    fstat(fileno(file), &buffer);
    return buffer.st_size;
}

/**
 * @brief Check if value lies between left and right (left can be bigger than right).
 * 
 * @param value pointer to check
 * @param left first bound
 * @param right second bound
 * @return bool
 */
static inline int in_bounds(const void * const value, const void * const left, const void * const right) {
    if (right < left) {
        return right <= value && value <= left;
    }
    return left <= value && value <= right;
}

int compare_line_pointers(const void* ptr_a, const void* ptr_b) {
    //* There can be overflow so it is better to write individual if-statements.
    const wchar_t* start_a = ((Charline*)ptr_a)->begin();
    const wchar_t* start_b = ((Charline*)ptr_b)->begin();
    if (start_a > start_b) return 1;
    if (start_a < start_b) return -1;
    return 0;
}

int wlinecmp(const wchar_t* start_a, const wchar_t* end_a, const wchar_t* start_b, const wchar_t* end_b) {
    const wchar_t *id_a = start_a, *id_b = start_b;
    int step_a = (end_a > start_a) ? 1 : -1;
    int step_b = (end_b > start_b) ? 1 : -1;
    
    while (in_bounds(id_a, start_a, end_a) && in_bounds(id_b, start_b, end_b)) {
        while (!iswsortable(*id_a) && in_bounds(id_a, start_a, end_a)) id_a += step_a;
        while (!iswsortable(*id_b) && in_bounds(id_b, start_b, end_b)) id_b += step_b;
    
        if (!in_bounds(id_a, start_a, end_a) || !in_bounds(id_b, start_b, end_b)) break;

        int difference = *id_a - *id_b;
        if (difference) return difference;

        id_a += step_a; id_b += step_b;
    }
    
    return in_bounds(id_a, start_a, end_a) - in_bounds(id_b, start_b, end_b);
}

int compare_lines(const void* void_a, const void* void_b) {
    Charline* line_a = (Charline*)void_a;
    Charline* line_b = (Charline*)void_b;
    return wlinecmp(line_a->begin(), line_a->end() - 1, line_b->begin(), line_b->end() - 1);
}

int compare_reverse_lines(const void* void_a, const void* void_b) {
    Charline* line_a = (Charline*)void_a;
    Charline* line_b = (Charline*)void_b;
    return wlinecmp(line_a->end() - 1, line_a->begin(), line_b->end() - 1, line_b->begin());
}

int read_file(const char* file_name, Charline* *text, wchar_t* *buffer, int* error_code) {
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text,      "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(buffer,    "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);

    FILE* file = fopen(file_name, "r");
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, ENOENT);

    int file_size = get_file_length(file);

    int line_count = 1;

    setvbuf(file, NULL, _IOFBF, file_size * sizeof(**buffer));

    // TODO: Rewrite function with open(), read() and close() so it is possible to read whole file content at once.

    *buffer = (wchar_t*)calloc(file_size, sizeof(**buffer));
    _LOG_FAIL_CHECK_(*buffer, "error", ERROR_REPORTS, fclose(file);return READING_FAILURE;, error_code, ENOMEM);
    for (int char_id = 0; char_id < file_size; char_id++) {
        (*buffer)[char_id] = fgetwc(file);
        if ((*buffer)[char_id] == (wchar_t)'\n') {
            line_count++;
            (*buffer)[char_id] = (wchar_t)'\0';
        }
    }

    fclose(file);

    *text = (Charline*)calloc(line_count, sizeof(**text));
    _LOG_FAIL_CHECK_(*text, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, ENOMEM);
    (*text)[0] = *buffer;
    int line_id = 0;
    for (int char_id = 0; char_id < file_size; char_id++) {
        if ((*buffer)[char_id] == (wchar_t)'\0') {
            (*buffer)[char_id] = 0;
            (*text)[++line_id] = (*buffer) + char_id + 1;
        }
    }

    return line_count;
}

void write_file(const char* file_name, const Charline* const text, int text_length, int* error_code) {
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text, "error", ERROR_REPORTS, return;, error_code, EFAULT);

    FILE* file = fopen(file_name, "w");
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return;, error_code, ENOENT);

    size_t file_length = 0;
    for (int line_id = 0; line_id < text_length; line_id++) {
        file_length += text[line_id].length + 1;
    }
    
    setvbuf(file, NULL, _IOFBF, file_length * sizeof(**text));

    //TODO: Optimize IO with syscall?

    for (int line_id = 0; line_id < text_length; line_id++) {
        int char_id = 0;
        while (text[line_id][char_id] != 0) {
            fputwc(text[line_id][char_id], file);
            char_id++;
        }
        fputwc('\n', file);
    }

    fclose(file);
}
