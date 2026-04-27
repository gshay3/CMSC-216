/* exmaple_driver.c 
 * A simple driver program that demonstrates how to test the basic functionalities of your concurrent 
 * linked-list implementation. This program initializes the list, performs several insertions and 
 * deletions, and displays the list contents using multiple threads to illustrate concurrent access.
 */


/* Include the necessary headers */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "threads.h"

/* A simple comparison function for integer data
 * It returns 0 if values are equal, a positive value if a > b, or a negative value if a < b
 */
int int_cmp(void* a, void* b) {
    return *(int*)a - *(int*)b;
}

void print_int(void* i){
	printf("%d", *(int *)i);
}

void* thread_func(void* arg) {
	/* Cast the void pointer back to a LinkedList pointer */
    LinkedList* list = (LinkedList*) arg;
		
	/* Sample data to be used in the list */
    int data1 = 1, data2 = 2, data3 = 3;

    /* Insert nodes into the linked list
     * Demonstrates the insertion of multiple data points
	 */
    insert_node(list, &data1);
    insert_node(list, &data2);
    insert_node(list, &data3);
	
	/* Delete a node */
    delete_node(list, &data2, int_cmp);

    /* Search for a node and print its data
     * Demonstrates search functionality and thread safety in accessing nodes
	 */
    Node* node = search_node(list, &data1, int_cmp);
    if (node) {
        printf("Node found: %d\n", *(int*)get_data(node));
    }
	
    /* Display the current state of the list
     * Helps visualize the effect of concurrent operations
	 */
    display_list(list, print_int);
    return NULL;
}

int main() {
    /* Declare threads */
    pthread_t threads[2];
    LinkedList list;
	
    /* Initialize the linked list */
    init_list(&list);

    /* Create multiple threads to demonstrate concurrency */
    for (int i = 0; i < 2; ++i) {
        pthread_create(&threads[i], NULL, thread_func, &list);
        printf("thread created\n");
    }

    /* Wait for all threads to finish
     * Ensures that all operations complete before main thread proceeds
	 */
    for (int i = 0; i < 2; ++i) {
        printf("pthread_join called\n");
        pthread_join(threads[i], NULL);
        printf("successful join\n");
    }
	
	printf("Final list: \n");
    display_list(&list, print_int);
	
	/* Clean up and destroy the linked list */
    destroy_list(&list);
}