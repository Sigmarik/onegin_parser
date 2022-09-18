/**
 * @file txtproc.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Module for processing text files.
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TXTPROC_H
#define TXTPROC_H

#include <cstddef>
#include <wchar.h>

enum READING_STATUSES {
    READING_SUCCESS = 0,
    READING_FAILURE = -1,
};

//TODO: Make separate structures to store length of the string (arguable).
typedef wchar_t* str;
typedef const wchar_t* cstr; // TODO: I'd rather write const str, since
                             //       it's not very obvious what cstr means
                             //       (my first intuition was C-string)

/**
 * @brief Compares two strings as strcmp().
 * 
 * @param a first string (as void*)
 * @param b second string (as void*)
 * @return int strcmp(a, b)
 */
int compare_lines(const void* a, const void* b);

/**
 * @brief Compares two inverted strings as strcmp().
 * 
 * @param a first string (as void*)
 * @param b second string (as void*)
 * @return int strcmp(reverse_a, reverse_b)
 */
int compare_reverse_lines(const void* a, const void* b);

/**
 * @brief Reads text file and saves its content.
 * 
 * @param[in] file_name name of the file to read
 * @param[out] text array of links to lines that will be filled
 * @param[out] buffer the string whole file will be written to
 * @param[out] error_code where to put error codes
 * @returns text length if reading was successful and READING_FAILURE otherwise
 */
int read_file(const char* file_name, str* *text, str *buffer, int* error_code = NULL);

/**
 * @brief Writes text to file.
 * 
 * @param file_name name of the file to write text into
 * @param text text to write
 * @param text_length number of lines in the text
 * @param error_code where to put error codes
 */
void write_file(const char* file_name, const cstr* text, int text_length, int* error_code = NULL);

#endif
