/*
 * Description:
 * This file implements a thread-safe singly linked list using POSIX read-write
 * locks. It provides basic operations including initialization, insertion,
 * deletion, search, traversal, and cleanup, allowing safe concurrent access
 * by multiple reader and writer threads.
 * 
 * Author: Griffin Shay
 * Date: 12/11/2024
 */

#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Initialize the linked list and associated locks
void init_list(LinkedList *list) 
{
    if (!list) 
    {
        return; // Check for null list
    }
    list->head = NULL;

    pthread_rwlock_init(&list->rwlock, NULL);
}

// Destroy the linked list and associated locks
void destroy_list(LinkedList *list) 
{
    if (!list) 
    {
        return; // Check for null list
    }

    pthread_rwlock_wrlock(&list->rwlock);
    Node *current = list->head;
    while (current) 
    {
        Node *next = current->next;
        free(current);  // Free each node
        current = next;
    }
    list->head = NULL;
    
    pthread_rwlock_unlock(&list->rwlock);
    pthread_rwlock_destroy(&list->rwlock);  // Destroy the read-write lock
}

// Insert a new node into the linked list
int insert_node(LinkedList *list, void *data) 
{
    if (!list || !data) 
    {
        return -1; // Validate input parameters
    }

    Node *new_node = (Node *)malloc(sizeof(Node));

    if (!new_node) 
    {
        return -1;  // Memory allocation failure
    }

    new_node->data = data;

    pthread_rwlock_wrlock(&list->rwlock);  // Acquire write lock for exclusive access

    // Insert the new node at the head of the list
    new_node->next = list->head;
    list->head = new_node;

    pthread_rwlock_unlock(&list->rwlock);  // Release the write lock

    return 0;
}

// Delete a node from the linked list
int delete_node(LinkedList *list, void *data, int (*cmp)(void *, void *)) 
{
    if (!list || !data || !cmp) 
    {
        return 0; // Validate input parameters
    }

    pthread_rwlock_wrlock(&list->rwlock);  // Acquire write lock for exclusive access

    Node *current = list->head;
    Node *prev = NULL;

    while (current) 
    {
        if (cmp(current->data, data) == 0) // Match found
        { 
            if (prev) 
            {
                prev->next = current->next;  // Bypass the current node
            } 
            else 
            {
                list->head = current->next;  // Update the head if first node is removed
            }
            current->data = NULL;
            pthread_rwlock_unlock(&list->rwlock);  // Release the write lock
            return 1;
        }
        prev = current;
        current = current->next;  // Move to the next node
    }

    pthread_rwlock_unlock(&list->rwlock);  // Release the write lock
    return 0;  // Node not found
}

// Search for a node in the linked list
Node* search_node(LinkedList *list, void *data, int (*cmp)(void *, void *)) 
{
    if (!list || !data || !cmp) 
    {
        return NULL; // Validate input parameters
    }

    pthread_rwlock_rdlock(&list->rwlock);  // Acquire read lock for concurrent access
    Node *current = list->head;
    
    while (current) 
    {
        if (cmp(current->data, data) == 0) //Match found
        { 
            Node* result = current;
            pthread_rwlock_unlock(&list->rwlock);  // Release the read lock
            return result;
        }
        current = current->next;  // Move to the next node
    }

    pthread_rwlock_unlock(&list->rwlock);  // Release the read lock
    return NULL;  // Node not found
}

// Retrieve data from a node
void* get_data(Node *node) 
{
    if (!node)
    {
        return NULL;  // Null check
    }

    return node->data;
}

// Display the linked list
void display_list(LinkedList *list, void (*print_func)(void *)) 
{
    if (!list || !print_func) 
    {
        return; // Validate input parameters
    }

    pthread_rwlock_rdlock(&list->rwlock);

    Node *current = list->head;
    while (current) 
    {
        print_func(current->data);  // Print each node's data using the provided function
        current = current->next;  // Move to the next node
    }

    printf("\n");

    pthread_rwlock_unlock(&list->rwlock);  // Release the write lock
}
