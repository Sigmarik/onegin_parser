/**
 * @file main.c
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Program for reading text file and sorting lines.
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/util/dbg/debug.h"
#include "lib/util/argparser.h"
#include "lib/txtproc.h"


/**
 * @brief Prints poem of owls.
 * 
 * @param argc unimportant
 * @param argv unimportant
 * @param argument unimportant
 */
void print_owl(const int argc, void** argv, const char* argument);

/**
 * @brief Prints program label and build date/time to console.
 * 
 */
void print_label();

static int log_threshold = 1;
static void* __log_threshold_array[1] = {&log_threshold};

static const char* text_source_name = "onegin.txt";
static void* __text_source_name_array[1] = {&text_source_name};

static const int NUMBER_OF_TAGS = 3;
static const struct ActionTag LINE_TAGS[NUMBER_OF_TAGS] = {
    {
        .name = {'O', "owl"}, 
        .action = {
            .parameters = {}, 
            .parameters_length = 0, 
            .function = print_owl, 
        },
        .description = "prints 10 owls on the screen."
    },
    {
        .name = {'I', "importance"}, 
        .action = {
            .parameters = __text_source_name_array,
            .parameters_length = 1, 
            .function = edit_int,
        },
        .description = "(in the form of -I[int]) sets log\n"
                        "    threshold to specified number.\n" 
                        "    Does not check if integer is specified."
    },
    {
        .name = {'O', "open"}, 
        .action = {
            .parameters = __log_threshold_array,
            .parameters_length = 1, 
            .function = edit_string,
        },
        .description = "(in the form of -O[name]) makes program open\n"
                        "    specified file instead of the default one."
    },
};

/**
 * @brief Main function of the program. Reads 3 doubles from stdio and prints the solution of the equation.
 * 
 * @return exit status (0 if successful)
 */
int main(const int argc, const char** argv) {
    atexit(log_end_program);

    parse_args(argc, argv, NUMBER_OF_TAGS, LINE_TAGS);
    log_init("program_log.log", log_threshold, &errno);
    print_label();

    log_printf(STATUS_REPORTS, "status", "Reading file %s...\n", text_source_name);

    char** text = {};
    int text_size = read_file(text_source_name, &text, &errno);

    if (text_size == READING_FAILURE) {
        log_printf(ERROR_REPORTS, "error", "Failed to read file %s. Terminating.\n", text_source_name, &text);
        return EXIT_FAILURE;
    }

    log_printf(STATUS_REPORTS, "status", "Descovered %d lines of text.\n", text_size);

    log_printf(STATUS_REPORTS, "status", "Writing the direct copy...\n");

    write_file("text_copy.txt", text, text_size, &errno);

    log_printf(STATUS_REPORTS, "status", "Sorting...\n");

    qsort(text, text_size, sizeof(text[0]), compare_lines);

    log_printf(STATUS_REPORTS, "status", "Exporting sorted lines...\n");

    write_file("text_sorted.txt", text, text_size, &errno);

    log_printf(STATUS_REPORTS, "status", "Re-sorting...\n");

    qsort(text, text_size, sizeof(text[0]), compare_reverse_lines);

    log_printf(STATUS_REPORTS, "status", "Exporting inv-sorted lines...\n");

    write_file("text_inv_sorted.txt", text, text_size, &errno);

    return EXIT_SUCCESS;
}

// Офигенно, ничего не менять.
// Дополнил сову, сорри.
void print_owl(const int argc, void** argv, const char* argument) {
    printf("-Owl argument detected, dropping emergency supply of owls.\n");
    for (int i = 0; i < 10; i++) {
        printf("  A_,,,_A           \n");
        printf(" ((O)V(O))          \n");
        printf("(\"\\\"|\"|\"/\")   \n");
        printf(" \\\"|\"|\"|\"/     \n");
        printf("   \"| |\"          \n");
        printf("    ^ ^             \n");
    }
}

void print_label() {
    printf("Text processor by Ilya Kudryashov.\n");
    printf("Program opens example text and prints it.\n");
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n", __DATE__, __TIME__);
}