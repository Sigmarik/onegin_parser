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

struct Charline {
    const wchar_t* sequence;
    size_t length = 0;

    Charline operator=(const wchar_t * const new_string) { sequence = new_string; length = wcslen(sequence); return *this; }

    wchar_t operator[](int index) const { return sequence[index];                            }
    Charline operator+(int delta) const { return Charline{sequence + delta, length - delta}; }
    Charline operator-(int delta) const { return Charline{sequence - delta, length + delta}; }
    wchar_t operator*()           const { return *sequence;                                  }

    const wchar_t* begin() const { return sequence; }
    const wchar_t* end() const { return sequence + length; }
};

enum READING_STATUSES {
    READING_SUCCESS = 0,
    READING_FAILURE = -1,
};

/**
 * @brief Compare two pointer to the starts of the line as is.
 * 
 * @param ptr_a first line
 * @param ptr_b second line
 * @return int ptr_a - prt_b
 */
int compare_line_pointers(const void* ptr_a, const void* ptr_b);

/**
 * @brief Compare two character sequences iterating from start to end ignoring punctuation characters.
 * 
 * @param start_a start of the first sequence
 * @param end_a end of the first sequence
 * @param start_b start of the second sequence
 * @param end_b end of the second sequence
 * @return int as if sequences were pushed into strcmp()
 */
int wlinecmp(const wchar_t* start_a, const wchar_t* end_a, const wchar_t* start_b, const wchar_t* end_b);

/**
 * @brief Compare two strings as strcmp() ignoring punctuation.
 * 
 * @param a first string (as void*)
 * @param b second string (as void*)
 * @return int strcmp(a, b)
 */
int compare_lines(const void* a, const void* b);

/**
 * @brief Compare two inverted strings as strcmp() ignoring punctuation.
 * 
 * @param a first string (as void*)
 * @param b second string (as void*)
 * @return int strcmp(reverse_a, reverse_b)
 */
int compare_reverse_lines(const void* a, const void* b);

/**
 * @brief Read text file and save its content.
 * 
 * @param[in] file_name name of the file to read
 * @param[out] text array of links to lines that will be filled
 * @param[out] buffer the string whole file will be written to
 * @param[out] error_code where to put error codes
 * @returns text length if reading was successful and READING_FAILURE otherwise
 */
int read_file(const char* file_name, Charline* *text, wchar_t* *buffer, int* error_code = NULL);

/**
 * @brief Write text to file.
 * 
 * @param file_name name of the file to write text into
 * @param text text to write
 * @param text_length number of lines in the text
 * @param error_code where to put error codes
 */
void write_file(const char* file_name, const Charline* const text, int text_length, int* error_code = NULL);

#endif
