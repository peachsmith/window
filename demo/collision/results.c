#include "demo/collision/collision.h"

#include <stdlib.h>

collision_list *col_create_list()
{
    int cap;              // initial capacity
    col_res *data;        // dynamic array
    collision_list *list; // wrapper struct

    // Allocate memory for an array with an initial capacity of 10.
    cap = 10;
    data = (col_res *)malloc(sizeof(col_res) * cap);
    if (data == NULL)
    {
        return NULL;
    }

    // Allocate memory for the wrapper struct.
    list = (collision_list *)malloc(sizeof(collision_list));
    if (list == NULL)
    {
        free(data);
        return NULL;
    }

    // Populate the fields of the wrapper struct.
    list->cap = cap;
    list->data = data;
    list->count = 0;

    return list;
}

void col_destroy_list(collision_list *list)
{
    if (list == NULL)
    {
        return;
    }

    // Free the array.
    if (list->data != NULL)
    {
        free(list->data);
    }

    // Free the wrapper struct.
    free(list);
}

void col_clear_list(collision_list *list)
{
    if (list == NULL)
    {
        return;
    }

    list->count = 0;
}

int col_push_result(collision_list *list, col_res *result)
{
    if (list == NULL || result == NULL)
    {
        return 0;
    }

    // If we've reached capacity, resize the array.
    if (list->count >= list->cap)
    {
        int new_cap;
        col_res *new_data;

        new_cap = list->cap + list->cap / 2;
        new_data = (col_res *)realloc(list->data, new_cap);
        if (new_data == NULL)
        {
            free(list->data);
            list->data = NULL;
            return 0;
        }

        list->data = new_data;
        list->cap = new_cap;
    }

    // Copy the contents of the new result into the array.
    list->data[list->count++] = *result;

    return 1;
}
