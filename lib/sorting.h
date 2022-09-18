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

//! TODO: Make a typedef for comparator. (use _t suffix)
//! It will just ruin function declaration's simillarity to qsort's declaration.
// TODO: Quite the opposite, actually, look at GNU C lib's implementation (the
//       one, your are most likely using anyway) of stdlib's qsort: it has `compar_fn_t'
//
//       Which, in my opinion, makes signature look cleaner and function easier to use.

/**
 * @brief (wraps void _msort()) Merge sort that works (from the user's perspective) like qsort.
 * 
 * @param array pointer to the first element of the array
 * @param length array element count
 * @param cell_size single element's size
 * @param comparison comparison function
 */
void msort(void* array, int length, size_t cell_size, 
           int (*comparison)(const void* ptr_a, const void* ptr_b));
//* It may or may not be useful to make this function inline as it 
//* does not use any recursion and generally can be replaced with simple defile.
//* Modern compilers, though, probably automatically detect things like this one.

#endif
