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
 * @brief Sort the array with the merge sort algorithm.
 * 
 * @param array pointer to the first element of the array
 * @param length array element count
 * @param cell_size single element's size
 * @param comparison comparison function
 */
void msort(void* array, int length, size_t cell_size, __compar_fn_t comparison);
//* It may or may not be useful to make this function inline as it 
//* does not use any recursion and generally can be replaced with simple defile.
//* Modern compilers, though, probably automatically detect things like this one.

#endif
