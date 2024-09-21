/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/

#ifndef T_STRING
#define T_STRING

typedef char *string;

#endif // T_STRING

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

// TODO: Define exactly what a `struct List_Repr` is in list.c
typedef struct List_Repr *list;


// TODO: Implement all the functions below in list.c

/***************************** GENERAL INTERFACE ******************************/

/**
 * list_create
 *
 * @brief Allocate the required memory for a new list.
 * 
 * @return The new list. If there's an error (e.g. out of memory), return NULL.
 */
list list_create(void);

/**
 * list_destroy
 *
 * @brief Free all memory associated with the given list.
 *
 * @param list The list to be destroyed. If it's NULL, do nothing.
 */
void list_destroy(list);

/**
 * list_is_empty
 * 
 * @brief Determines whether a given list is empty or not.
 * 
 * @param list The list to check. If it's NULL, the list is empty.
 * @return     True if there are no items in the list
 *             False if there are items in the list
 */
bool list_is_empty(list);

/**
 * list_length
 *
 * @brief Returns the number of items in the list.
 * 
 * @param list The list to check. If it's NULL, the list is empty.
 * @return     The number of items in the list.
 */
size_t list_length(list);


/****************************** STACK INTERFACE *******************************/

/**
 * list_push
 * 
 * @brief Push an item onto the top of the stack.
 * 
 * @param   list The stack to push. If it's NULL, do nothing.
 * @param string The item to be pushed onto the stack. If it's NULL, do nothing.
 */
void list_push(list, string);

/**
 * list_pop
 * 
 * @brief Pop an item from the top of the stack.
 * 
 * @param list The stack to pop. If it's NULL, do nothing.
 * @return     The item popped from the top of the stack. If the stack is
 *             empty, return NULL.
 */
string list_pop(list);


/****************************** QUEUE INTERFACE *******************************/

/**
 * list_enqueue
 * 
 * @brief Enqueue an item at the rear of the queue.
 * 
 * @param   list The queue to enqueue. If it's NULL, do nothing.
 * @param string The item to be enqueued.  If it's NULL, do nothing.
 */
void list_enqueue(list, string);

/**
 * list_dequeue
 * 
 * @brief Dequeue an item from the head of the queue.
 * 
 * @param list The queue to dequeue. If it's NULL, do nothing.
 * @return     The item dequeued from the head of the queue. If the queue is
 *             empty, return NULL.
 */
string list_dequeue(list);


/******************************* SET INTERFACE ********************************/

 /**
 * list_add
 * 
 * @brief Add an item to the set, if it is not already in the set.
 * 
 * @param   list The set to add to. If it's NULL, do nothing.
 * @param string The item to be added.  If it's NULL, or already in the set, 
 *               do nothing.
 */
void list_add(list, string);

 /**
 * list_remove
 * 
 * @brief Remove an item from the set.
 * 
 * @param   list The set to remove from. If it's NULL, do nothing.
 * @param string The item to be removed. If it's NULL, or not in the set, 
 *               do nothing.
 */
void list_remove(list, string);

 /**
 * list_contains
 * 
 * @brief Check if an item is a member of the set.
 * 
 * @param   list The set to check. If it's NULL, return false.
 * @param string The item to check. If it's NULL, return false.
 * @return       true if the item is in the set, false otherwise.
 */
bool list_contains(list, string);

#endif // LIST_H

/*************************** DO NOT CHANGE THIS FILE ***************************
 *                         THIS FILE IS NOT SUBMITTED                          *
 *             AND WILL BE SUPPLIED AS IS DURING TESTING AND MARKING           *
 *************************** DO NOT CHANGE THIS FILE ***************************/
