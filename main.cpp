/**
 * @file main.c
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Program for reading text file and sorting its lines.
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <clocale>
#include "lib/util/dbg/debug.h"
#include "lib/util/argparser.h"
#include "lib/txtproc.h"
#include "lib/sorting.h"

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

/**
 * @brief Stores text as a bunch of lines.
 * 
 * @param lines pointers to characters stored in charbuffer making lines of text
 * @param charbuffer buffer with concatenated together lines of text
 */
struct Text {
    str* lines = NULL;
    str charbuffer = NULL;
};

/**
 * @brief Frees internal buffers of the Text struct.
 * 
 * @param text text to free
 */
void free_text(Text* text, int* err_code = NULL);

// TODO: Why though? (see line 60)
static const size_t MAX_SOURCE_NAME_LENGTH = 1000;

static int log_threshold = 1;
static void* TEMP_log_threshold_array[] = {&log_threshold};

// TODO: Just write empty brackets[], it will calculate size for you!
static char text_source_name[MAX_SOURCE_NAME_LENGTH] = "onegin.txt";
static void* TEMP_text_source_name_array[] = {&text_source_name};

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
        .name = {'I', ""}, 
        .action = {
            .parameters = TEMP_log_threshold_array,
            // TODO: Yeah, void** params created need
            //       for this TEMP_* stuff.
            //       
            //       I think I would instead make
            //       macro for defining ActionTags
            //
            // ==> Like:     v~~~~~~~~~~~~~~~~~~~~~~~~ name of the action
            // DEFINE_ACTION(CHANGE_LOG_THRESHOLD, { 'I', "" },
            //               ACTION(edit_int, &log_threshold))
            // 
            // Which would internally create all the
            // necessary TEMP_* cringe.
            //
            // ==> And then do:
            // const ActionTag actions[] = {
            //     CHANGE_LOG_THRESHOLD,
            //     ...
            // };
            //
            // This approach would make you name your actions,
            // which, I think, is a good thing. 
            //
            // At least, you'll have something meaningful 
            // at top level instead of all TEMP_* boilerplate.
            //
            // Consider, if you like it.

            .parameters_length = 1, 
            .function = edit_int,
        },
        .description = "sets log\n"
                        "    threshold to specified number.\n" 
                        "    Does not check if integer is specified."
    },
    {
        .name = {'R', ""}, 
        .action = {
            .parameters = TEMP_text_source_name_array,
            .parameters_length = 1, 
            .function = edit_string,
        },
        .description = "makes program open\n"
                        "    specified file instead of the default one."
    },
};

// TODO: No reason to write "main function of the program", it's obvious
//       to any developer reading your code, just clutters documentation.
//
//       In general, it's advised by many to write documentation in imperative.
//
//       For example, instead of:
//           Function that reads file and puts...
// 
//       You could write:
//           Read file and put...
//
//       Note absence of "s" after verbs, it's because we are no longer
//       describing function in third person, we're just telling user
//       what's gonna happen when he calls it!
//
//       This "s" absence is common for all imperative sentences.
//       And, I general, is consider a good style, which make documentation
//       less wishy-washy and more straight-to-the-point.
//
// TODO: Also need for documentation of "main" is debatable :)
//       I mean, it should match with program description (for example, README)
//
//       And all technicalities about main (like that it returns error code) 
//       are well-known by all C/C++ programmers (there's no other way if they
//       wrote at least one program) and well-documented in standard library.
/**
 * @brief Main function of the program. Reads file and puts its multiple forms into another file.
 * 
 * @return exit status (0 if execution was successful)
 */
int main(const int argc, const char** argv) {
    atexit(log_end_program);

    setlocale(LC_ALL,"C.UTF-8");
    setlocale(LC_CTYPE,"C.UTF-8");

    parse_args(argc, argv, NUMBER_OF_TAGS, LINE_TAGS);
    log_init("program_log.log", log_threshold, &errno);
    print_label();

    log_printf(STATUS_REPORTS, "status", "Reading file %s...\n", text_source_name);

    struct Text text;
    int text_size = read_file(text_source_name, &text.lines, &text.charbuffer, &errno);
    _LOG_FAIL_CHECK_(!errno, "error", ERROR_REPORTS, return EXIT_FAILURE, &errno, EIO);

    if (text_size == READING_FAILURE) {
        log_printf(ERROR_REPORTS, "error", "Failed to read file %s. Terminating.\n", text_source_name, &text);
        return EXIT_FAILURE;
    }

    log_printf(STATUS_REPORTS, "status", "Descovered %d lines of text.\n", text_size);

    log_printf(STATUS_REPORTS, "status", "Writing the direct copy...\n");
    write_file("text_copy.txt", text.lines, text_size, &errno);
    _LOG_FAIL_CHECK_(!errno, "error", ERROR_REPORTS, return EXIT_FAILURE, &errno, EIO);
    // TODO:         ^~~~~~~ errno checking is very common in your program,
    //               consider extracting this in another macro!
    //
    // Or, maybe, even couple it with logging and action itself and extract pattern:
    //     log_action();
    //     action();
    //     check_success();
    //
    // Which is very common on its own (5 occurences just in this function).
    //
    // For example, you could make something like:
    //     DO_AND_CHECK_SUCCESS(ACTION(write_file), "text_copy.txt", ...);
    //
    // Expand in:
    //     log_printf(STATUS_REPORTS, "status", "write_file...");
    //     write_file(...);
    //     _ON_ERROR_(return EXIT_FAILURE); // Shorthand for _LOG_FAIL_CHECK_i
    //                                         i suggested earlier.
    //
    // It would make code cleaner, hence more readable.
                         

    log_printf(STATUS_REPORTS, "status", "Sorting...\n");
    msort(text.lines, text_size, sizeof(*text.lines), compare_lines);

    log_printf(STATUS_REPORTS, "status", "Exporting sorted lines...\n");
    write_file("text_sorted.txt", text.lines, text_size, &errno);
    _LOG_FAIL_CHECK_(!errno, "error", ERROR_REPORTS, return EXIT_FAILURE, &errno, EIO);

    log_printf(STATUS_REPORTS, "status", "Re-sorting...\n");
    msort(text.lines, text_size, sizeof(*text.lines), compare_reverse_lines);

    log_printf(STATUS_REPORTS, "status", "Exporting inv-sorted lines...\n");
    write_file("text_inv_sorted.txt", text.lines, text_size, &errno);
    _LOG_FAIL_CHECK_(!errno, "error", ERROR_REPORTS, return EXIT_FAILURE, &errno, EIO);

    free_text(&text);
    // TODO: So, if anything fails before this free, memory isn't gonna get freed?

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
        // TODO: There's a C++11 feature called "raw strings",
        //       which allows you to include quotes without escaping.
        //
        //       Even though, it's technically C++, you're
        //       allowed to use it if you want to.
        // 
        // It would make owl look cleaner in source code:
        //       R"(  A_,,,_A   \n)"
        //       R"( ((O)V(O))  \n)"
        //       R"(("\"|"|"/") \n)"
        //       R"( \"|"|"|"/  \n)"
        //       R"(   "| |"    \n)"
        //       R"(    ^ ^     \n)"
    }
}

void print_label() {
    printf("Text processor by Ilya Kudryashov.\n");
    printf("Program opens example text and prints it.\n");
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n", __DATE__, __TIME__);
}

void free_text(Text* text, int* err_code) {
    _LOG_FAIL_CHECK_(text->charbuffer, "error", ERROR_REPORTS, return;, err_code, EFAULT);
    _LOG_FAIL_CHECK_(text->lines, "error", ERROR_REPORTS, return;, err_code, EFAULT);
    // TODO: I think, you revoked this syntax (see line 161)    ^

    free(text->charbuffer); text->charbuffer = NULL;
    free(text->lines);      text->lines      = NULL;
}
