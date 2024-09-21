#include <stdio.h>
#include <string.h>
#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>

#ifndef T_STRING
#define T_STRING

typedef char *string;

#endif

#define LIST_H

typedef struct List_Repr{
    struct List_Repr *next;
    struct List_Repr *prev;
    char *data;
    struct List_Repr *head;
    struct List_Repr *tail;
}List_Repr;

list list_create(void){
    List_Repr* new_list = (List_Repr*)malloc(sizeof(List_Repr));
    assert(new_list != NULL);
    new_list -> next =NULL;
    new_list -> prev =NULL;
    new_list -> data =NULL;
    return new_list;
}

void list_destroy(list list_to_destroy){
    while(list_to_destroy != NULL){
        List_Repr *current = list_to_destroy;
        list_to_destroy = list_to_destroy -> next;
        free(current);
        return;
    }
    free(list_to_destroy);
}

bool list_is_empty(list list_to_check){
    if(list_to_check == NULL||list_to_check -> next ==NULL){
        return true;
    }
    else{
        return false;
    }
}

size_t list_length(list list_to_calculate) {
    if (list_to_calculate == NULL) {
        return 0; 
    }

    List_Repr *current = list_to_calculate->next;
    int count = 0;
    while (current != NULL) {
        count++;
        if (current->next == NULL) {
            break; 
        }
        current = current->next;
    }
    return count;
}


//fuctions about stack
void list_push(list stack, string item) {
    if (stack == NULL || item == NULL) {
        return;
    }

    List_Repr *new_node = (List_Repr *)malloc(sizeof(List_Repr));
    assert(new_node != NULL);

    char *item_copy = strdup(item);
    assert(item_copy != NULL);

    new_node->data = item_copy;
    new_node->next = stack->next;
    stack->next = new_node;
}
    // push the element into the head of the list,
    // use the tail of the list as the bottem of the stack//


string list_pop(list stack){
    if(stack == NULL || stack->next == NULL){
        return NULL;
    }
    else{
        List_Repr *top_node = stack->next;
        stack->next = top_node->next; // Updating the top pointer of the stack
        string item_pop = top_node->data; // Return pointer to data
        free(top_node); // Free the popped node
        return item_pop;
    }
}
//pop the first item at the list (as the top of the satck)

//fuctions about queue
void list_enqueue(list queue, string item){
    if( queue == NULL || item == NULL){
        return;
    }
    else {
        List_Repr *new_node = (List_Repr *)malloc(sizeof(List_Repr));
        assert(new_node != NULL);

        char *item_copy = strdup(item);
        assert(item_copy != NULL);

        new_node->data = item_copy;
        new_node->next = NULL;

        List_Repr *current = queue;
        while (current->next != NULL) {
            current = current->next;
        }
        current -> next = new_node;
        return;
    }
}
//enqueue the element at the tail of the list(also the tail of queue)

string list_dequeue(list queue){
    if(queue == NULL){
        return NULL;
    }

    List_Repr *first_node = queue->next;
    string item_dequeue = strdup(first_node->data);
    
    queue ->next = first_node -> next;
    free(first_node -> data);
    free(first_node);

    return item_dequeue;
    
}
//dequeue the first element of the list(as the head of the queue)

//fuctions about set
void list_add(list set, string item) {
    if (set == NULL || item == NULL) {
        return;
    }

    List_Repr *current = set->next;
    while (current != NULL) {
        if (strcmp(current->data, item) == 0) {
            return;
        }
        if (current->next == NULL) {
            break; 
        }
        current = current->next;
    }

    List_Repr *new_node = (List_Repr *)malloc(sizeof(List_Repr));
    assert(new_node != NULL);

    char *item_copy = strdup(item);
    assert(item_copy != NULL);

    new_node->data = item_copy;
    new_node->next = NULL;

    if (current == NULL) {
        set->next = new_node;
    } else {
        current->next = new_node; 
    }
}


// if the value of the node is unique,put it to the tail of the list

void list_remove(list set, string item) {
    if (item == NULL || set == NULL) {
        return;
    }

    List_Repr *current = set->next;
    List_Repr *prev = set;

    // Find the node to remove
    while (current != NULL && strcmp(current->data, item) != 0&&current->next != NULL) {
        prev = current;
        current = current->next;
    }

    // If the element is not found, return
    if (current == NULL) {
        return;
    }

    // If the element is found at the head of the list
    if (prev == NULL) {
        set = current->next;
        if (set != NULL) {
            set->prev = NULL;
        }
        free(current->data);
        free(current);
        return;
    }

    // If the element is found at the tail of the list
    if (current->next == NULL) {
        prev->next = NULL;
        free(current->data);
        free(current);
        return;
    }

    // If the element is found in the middle of the list
    prev->next = current->next;
    current->next->prev = prev;
    free(current->data);
    free(current);
}

bool list_contains(list set, string item){
    if(set == NULL || item == NULL){
        return false;
    }
    
    List_Repr *current = set->next; 
    while (current != NULL) {
        if (strcmp(item, current->data) == 0) {
            return true; 
        }
        current = current->next;
    }
    
    return false; 
}

string list_get(list list_to_access, size_t index) {
    // Check if the list is NULL
    if (list_to_access == NULL) {
        return NULL;
    }

    // Traverse the list to the specified index
    List_Repr *current = list_to_access->next;
    size_t current_index = 0;
    while (current != NULL && current_index < index) {
        current = current->next;
        ++current_index;
    }

    // Check if the index is out of bounds
    if (current == NULL) {
        return NULL;
    }

    // Return the item at the specified index
    return current->data;
}










