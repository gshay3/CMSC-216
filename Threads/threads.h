/* threads.h
 * This is a basic implementation of the header file for the THREADS project
 * Students should feel free to augment this file as necessary, but the basic
 * API functions must remain unchanged.
 * Note: Students are free to re-define or change the structure definitions as necessary.
 */

#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

/* Define the node structure for the linked list */
typedef struct Node {
    void *data;               /* Pointer to the data stored in the node */
    struct Node *next;        /* Pointer to the next node in the list */
} Node;

/* Define the linked list structure */
typedef struct LinkedList {
    Node *head;               /* Pointer to the first node in the list */
    pthread_rwlock_t rwlock;
} LinkedList;

/*
 *	init_list - Initializes the linked list. Sets up any necessary structures and prepares the list for use, 
 *  including the initialization of locks.
 *	Returns: void.
 *	Error Handling: Handles errors related to memory allocation and lock initialization, returning a status 
 *  code if failures occur.
 */
	void init_list(LinkedList *list);

/*
 *	destroy_list - Destroys the linked list. Frees all resources associated with the list and cleans up any 
 *  synchronization primitives.
 *  Returns: void.
 *  Error Handling: Ensures all nodes are freed, even if errors occur during destruction.
 */
	void destroy_list(LinkedList *list);

/*  
 *  insert_node - Inserts a new node with the given data into the linked list. This function must acquire a 
 *  write lock before performing the insertion to prevent concurrent write conflicts.
 *  Returns: 0 on success, -1 on failure (e.g., memory allocation failure).
 *  Edge Cases: Correctly handle inserting elements under high concurrency.
 */
	int insert_node(LinkedList *list, void *data);

/*  
 *  delete_node - Deletes a node with the specified data from the linked list, using the comparison 
 *  function ‘cmp‘ to find the correct node.  A write lock is required for this operation to maintain
 *  list integrity during modifications.
 *  Returns: If the node is found and removed successfully, it returns 1, otherwise 0. 
 *  Edge Cases: Handle cases where multiple delete requests occur for the same node simultaneously.
 */
	int delete_node(LinkedList *list, void *data, int (*cmp)(void *, void *));

/*  
 *  search_node - Searches for a node containing the specified data, using the comparison function 
 *  ‘cmp‘ to determine equality. This function should utilize a read lock, allowing multiple threads 
 *  to search concurrently without blocking each other.
 *  Returns: If found, returns a pointer to the node, otherwise returns NULL.
 * 	Edge Cases: Correctly handle searches for non-existent elements under high concurrency.
 */
	Node* search_node(LinkedList *list, void *data, int (*cmp)(void *, void *));

/*  
 *  get_data - 	Returns the data stored in the given node. This function is typically used after 
 *  retrieving a node using the search function to safely extract its data without exposing the 
 *  node’s internal structure.
 *  Returns: If the node pointer is not null, returns the node's data, otherwise returns NULL
 */
	void* get_data(Node *node);

   /*  
    *  display_list - Outputs the elements of the linked list, using the print function 
 	*  ‘print_func‘ to display individual elements. This operation should acquire a 
	*  read lock to ensure data consistency during the display process.
 	*  Returns: void.
 	*  Error Handling: Handle errors related to an empty list (null pointer)
	*/
void display_list(LinkedList *list, void (*print_func)(void*));

#endif /* THREADS_H */
