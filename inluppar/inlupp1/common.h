#pragma once
#include <stdbool.h>

// the union element type
typedef union elem elem_t;

union elem
{
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
};
/// A link in the linked structure
typedef struct link link_t;

struct link
{
    elem_t element;
    link_t *next;
};

/// Compares two elements and returns true if they are equal
typedef bool (*ioopm_eq_function)(elem_t a, elem_t b);