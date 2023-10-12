#pragma once

#include <stdbool.h>

typedef union
{
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool (*check_func)(char *);
typedef answer_t (*convert_func)(char *);

extern char *strdup(const char *);
int read_string(char *buf, int buf_siz);

/// @brief Checks if input is a number, [-n, n]
/// @param str Char to be checked
/// @return True if argument is a number
bool is_number(char *str);

/// @brief Checks if input string is empty
/// @param str Char to be ckecked
/// @return True if string is not empty
bool not_empty(char *str);

/// @brief Checks if input string is a shelf E.g. ("A13")
/// @param str String to be checked
/// @return True if input is in shelf-format
bool is_shelf(char *str);

/// @brief Main function for question to be asked in terminal, prints question and takes input
/// @param question The question to be printed in the terminal
/// @param check Function to check if input is correct
/// @param convert Function to convert input into desired format
/// @return answer_t, the input value
answer_t ask_question(char *question, check_func check, convert_func convert);

/// @brief Function to ask for a string
/// @param question The question to be answered in terminal
/// @return The input value
char *ask_question_string(char *question);

/// @brief Function to ask for an integer
/// @param question The question to be answered in terminal
/// @return The input integer
int ask_question_int(char *question);

/// @brief Function to ask for a shelf
/// @param question The question to be answered in terminal
/// @return The input shelf
char *ask_question_shelf(char *question);

/// @brief Function to ask for menu input
/// @return The input letter
char *ask_question_menu();

/// @brief Function to ask for a specific letter, all other is opposit
/// @param question The question to be answered in terminal
/// @return the input
char *ask_question_letter(char *question);