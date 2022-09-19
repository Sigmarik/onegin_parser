/**
 * @file debug.h
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Module with debugging information.
 * @version 0.1
 * @date 2022-08-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DEBUGGING_H
#define DEBUGGING_H

/* NDEBUG should be here-> */
//#define NDEBUG
#include <assert.h>
#include <errno.h>

#include "logger.h"

/**
 * @brief List of error types to put into errno.
 */
enum ERRORS {
    INPUT_ERROR = -1,
    NULLPTR_ERROR = -2,
    FILE_ERROR = -3,
};

#ifndef NDEBUG
/**
 * @brief print description to log file if equation fails.
 * 
 */
#define _LOG_FAIL_CHECK_(equation, tag, importance, action, errcode, errtype)                                         \
do {                                                                                                                  \
    if (!(equation)) {                                                                                                \
        int* errptr = errcode;                                                                                        \
        if (errptr) *(errptr) = (errtype);                                                                            \
        log_printf(importance, "error", "Equation %s in file %s at line %d failed.\n", #equation, __FILE__, __LINE__);\
        action;                                                                                                       \
    }                                                                                                                 \
} while(0)
#else
/**
 * @brief (DISABLED) print description to log file if equation fails.
 * 
 */
#define _LOG_FAIL_CHECK_(equation, tag, importance, action, errcode, errtype) equation
#endif


/**
 * @brief Print errno value and its description and close log file.
 */
void log_end_program();

/**
 * @brief End program if errno is not zero.
 * 
 */
#define _ABORT_ON_ERRNO_() _LOG_FAIL_CHECK_(!errno, "FATAL ERROR", ABSOLUTE_IMPORTANCE, exit(EXIT_FAILURE);, NULL, 0);

#endif