#include "linked_list.h"
#include "iterator.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <stddef.h>

/**
 * @file simple_linked_list.c
 * @author Anton Eckervald
 * @date 1 Sep 2021
 * @brief A simple linked list that implements parts of the interface
 * of `linked_list.h`.
 */

// This is a macro that is used in the functions that are not implemented,
// to have them fail at runtime.
#define NOT_SUPPORTED()                                                            \
    do                                                                             \
    {                                                                              \
        fprintf(stderr, "The simple linked list does not support %s\n", __func__); \
        assert(false);                                                             \
    } while (0)

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_fun)
{
    ioopm_list_t *result = calloc(1, sizeof(struct list));

    if (result)
    {
        // Initiate list values
        result->head = result->last = calloc(1, sizeof(link_t));
        result->size = 0;
        result->eq = eq_fun;
    }

    return result;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list);
}

link_t *link_create(elem_t element, link_t *next)
{
    link_t *result = malloc(sizeof(struct link));

    if (result)
    {
        result->element = element;
        result->next = next;
    }
    return result;
}

link_t *get_previous_link(ioopm_list_t *list, int index)
{
    link_t *prev = list->head;
    for (int i = 0; i < index; i++)
    {
        prev = prev->next;
    }
    return prev;
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t element)
{
    link_t *new_link = link_create(element, NULL);
    list->last->next = new_link;
    list->last = new_link;
    list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t element)
{
    list->head->next = link_create(element, list->head->next);
    if (ioopm_linked_list_size(list) == 1)
    {
        list->last = list->head->next;
    }
    list->size++;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t element)
{
    if (index == 0)
    {
        ioopm_linked_list_prepend(list, element);
    }
    else if (ioopm_linked_list_size(list) == index)
    {
        ioopm_linked_list_append(list, element);
    }
    else
    {
        link_t *prev = get_previous_link(list, index);
        prev->next = link_create(element, prev->next);
        list->size++;
    }
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    if (index == 0)
    {
        link_t *tmp = list->head->next;
        elem_t element = tmp->element;
        list->head->next = tmp->next;
        free(tmp);
        list->size--;
        return element;
    }

    link_t *prev = get_previous_link(list, index);
    link_t *current = prev->next;

    prev->next = current->next;
    elem_t element = current->element;
    free(current);
    if (index == ioopm_linked_list_size(list) - 1)
    {
        list->last = prev;
    }
    list->size--;
    return element;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    link_t *prev = get_previous_link(list, index);
    return prev->next->element;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    link_t *cursor = list->head->next;
    while (cursor)
    {
        if (list->eq(cursor->element, element))
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    link_t *current = list->head;
    while (current)
    {
        link_t *tmp = current;
        current = current->next;
        free(tmp);
    }
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra)
{
    link_t *cursor = list->head->next;
    while (cursor)
    {
        if (!prop(cursor->element, extra))
        {
            return false;
        }
        cursor = cursor->next;
    }
    return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra)
{
    link_t *cursor = list->head->next;
    while (cursor)
    {
        if (prop(cursor->element, extra))
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function fun, elem_t extra)
{
    link_t *cursor = list->head->next;
    while (cursor)
    {
        fun(&(cursor->element), extra);
        cursor = cursor->next;
    }
}

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    ioopm_list_iterator_t *iter = malloc(sizeof(ioopm_list_iterator_t));
    iter->current = list->head;
    iter->list = list;
    return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    return iter->current->next != NULL;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    iter->current = iter->current->next;
    return iter->current->element;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = iter->list->head;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    return iter->current->element;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}