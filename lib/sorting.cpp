#include "sorting.h"

#include <cstring>

void msort(void* array, int length, size_t cell_size, 
           int (*comparison)(const void*, const void*), void* buffer) {
    if (length <= 1) return;
    bool init_buffer = false;
    if (!buffer) {
        init_buffer = true;
        buffer = calloc(length, cell_size);
    }

    int mid = length / 2;
    msort(array,                          mid,          cell_size, comparison, buffer);
    msort((char*)array + mid * cell_size, length - mid, cell_size, comparison, (char*)buffer + mid * cell_size);

    int left_id = 0, right_id = mid;
    for (int id = 0; id < length; id++) {
        if (right_id < length && (left_id >= mid || 
                comparison((char*)array + left_id * cell_size, (char*)array + right_id * cell_size) > 0)) {
            memcpy((char*)buffer + id * cell_size, (char*)array + (right_id++) * cell_size, cell_size);
        } else {
            memcpy((char*)buffer + id * cell_size, (char*)array + (left_id++) * cell_size, cell_size);
        }
    }

    for (int id = 0; id < length; id++) {
        memcpy((char*)array + id * cell_size, (char*)buffer + id * cell_size, cell_size);
    }

    if (init_buffer) free(buffer);
}