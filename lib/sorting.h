/**
 * @file algo.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Robust replacement for <algorithm> library
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ALGO_H
#define ALGO_H

#include <stdlib.h>

/**
 * @brief Merge sort that works (from the user's perspective) like qsort.
 * 
 * @param array pointer to the first element of the array
 * @param length array element count
 * @param cell_size single element's size
 * @param comparison comparison function
 */
void msort(void* array, int length, size_t cell_size, 
           int (*comparison)(const void*, const void*), void* buffer = NULL);

#endif