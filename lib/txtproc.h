/**
 * @file txtproc.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief 
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TXTPROC_H
#define TXTPROC_H

#include <cstddef>

#define FILE_MAX_LENGTH 100000
#define LINE_MAX_LENGTH 1000

enum READING_STATUSES {
    READING_SUCCESS = 0,
    READING_FAILURE = -1,
};

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

//* There is a standard library called <algorithm>, but we are not allowed to use it :(.
/**
 * @brief Reverses the string.
 * 
 * @param string string to reverse
 * @param error_code variable to put error code into.
 */
void reverse(char* string, int* error_code = NULL);

/**
 * @brief Reads text file and saves its content.
 * 
 * @param[in] file_name name of the file to read
 * @param[out] text where to put file content
 * @param[out] error_code where to put error codes
 * @returns text length if reading was successful and READING_FAILURE otherwise
 */
int read_file(const char* file_name, char*** text, int* error_code = NULL);

/**
 * @brief Writes text to file.
 * 
 * @param file_name name of the file to write text into
 * @param text text to write
 * @param text_length number of lines in the text
 * @param error_code where to put error codes
 */
void write_file(const char* file_name, const char*const* text, int text_length, int* error_code = NULL);

#endif