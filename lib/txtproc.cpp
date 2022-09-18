#include "txtproc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cwctype>
#include <errno.h>

#include "util/dbg/debug.h"

/**
 * @brief Checks if character shold not be skipped suring sorting.
 *                                          TODO:  ^
 * @param character character to check
 * TODO:            ^~~~~~~~~~ a bit confusing tautology)
 * @return bool 1 if character must be sorted and 0 if it isn't
 */
static inline int iswsortable(const wchar_t character) {
    return iswalpha(character) || iswdigit(character);
}

//! TODO: Make single, mode generalized comparator for both cases (and two wrappers for qsort-capable versions).
//! It will be much slower because of the constant variable math and overcalls instead of the standard and fast -- and ++.

// TODO: I'm struggling to understand what is "constant variable math" and why is it slow.
//
//       And, please, drop your premature optimizations, as said Donald Knuth:
//       "Premature optimization is the root of all evil"
//
//       You should write clean and easy to use and modify code, than measure perfomance, and 
//       only after that start optimizing, not vice-versa.
//
//       It's especially true, because modern compilers are very smart, I doubt generalized
//       comparator will be slower than current one, because it will probably be inlined
//       by compiler anyway.
//
//       And, also, how you can talk about perfomance when you call wcslen a dozen times
//       during each comparison?) Your sort is like O(n^3), and you're worried about -- and ++.


int compare_lines(const void* void_a, const void* void_b) {
    cstr str_a = *(cstr*) void_a;
    cstr str_b = *(cstr*) void_b;

    size_t id_a = -1, id_b = -1;
    
    while (++id_a < wcslen(str_a) && ++id_b < wcslen(str_b)) {

        while (!iswsortable(str_a[id_a]) && id_a < wcslen(str_a)) ++id_a;
        while (!iswsortable(str_b[id_b]) && id_b < wcslen(str_b)) ++id_b;
    
        if (id_a >= wcslen(str_a) || id_b >= wcslen(str_b)) break;

        int difference = str_a[id_a] - str_b[id_b];
        if (difference) return difference;
    }
    
    return str_a[id_a] - str_b[id_b];
}

int compare_reverse_lines(const void* void_a, const void* void_b) {
    cstr str_a = *(cstr*) void_a;
    cstr str_b = *(cstr*) void_b;

    //! TODO: remove slow wcslen and replace it with length comparison from custon struct.
    //! No, I will not make a function in a library work with custom structures if all
    //! other functions work with standard C types as it will force the user into
    //! using my highly limited and task-specific structs.

    // TODO: Do you think "better library support" is good enough reason to worsen
    //       perfomance of your function by an order of magnitude?)
    //
    //       C library lack a huge amount of useful things, so it's very common
    //       to roll your own stuff (which is largely a central idea in our course) 
    //
    //       And it's not hard at all to transition back to stdlib's way anyway.

    int id_a = wcslen(str_a), id_b = wcslen(str_b);

    while (--id_a >= 0 && --id_b >= 0) {

        while (!iswsortable(str_a[id_a]) && id_a >= 0) --id_a;
        while (!iswsortable(str_b[id_b]) && id_b >= 0) --id_b;

        if (id_a < 0 || id_b < 0) break;

        int diff = str_a[id_a] - str_b[id_b];
        if (diff) return diff;
    }

    return id_a - id_b;
}

int read_file(const char* file_name, str* *text, str *buffer, int* error_code) {
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text,      "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(buffer,    "error", ERROR_REPORTS, return READING_FAILURE;, error_code, EFAULT);

    FILE* file = fopen(file_name, "r");
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, ENOENT);

    fseek(file, 0L, SEEK_END); // TODO: This belong in separate function count_file_length or something
    int file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    int line_count = 1;

    setvbuf(file, NULL, _IOFBF, file_size * sizeof(**buffer));

    *buffer = (str)calloc(file_size, sizeof(**buffer));
    _LOG_FAIL_CHECK_(*buffer, "error", ERROR_REPORTS, fclose(file);return READING_FAILURE;, error_code, ENOMEM);
    for (int char_id = 0; char_id < file_size; char_id++) {
        (*buffer)[char_id] = fgetwc(file);
        if ((*buffer)[char_id] == '\n') line_count++;
    }

    fclose(file);

    *text = (str*)calloc(line_count, sizeof(**text));
    _LOG_FAIL_CHECK_(*text, "error", ERROR_REPORTS, return READING_FAILURE;, error_code, ENOMEM);
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
    // TODO:                           ^~~~~ Why reapply const to "const string"?
    _LOG_FAIL_CHECK_(file_name, "error", ERROR_REPORTS, return;, error_code, EFAULT);
    _LOG_FAIL_CHECK_(text, "error", ERROR_REPORTS, return;, error_code, EFAULT);

    FILE* file = fopen(file_name, "w");
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return;, error_code, ENOENT);

    size_t file_length = 0;
    for (int line_id = 0; line_id < text_length; line_id++) {
        file_length += wcslen(text[line_id]) + 1;
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
