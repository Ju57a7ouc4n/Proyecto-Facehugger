#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init_list(list *l) {
    l->head = NULL;
    l->tail = NULL;
}

void add_node(list *l, const char *filename) {
    node *new_node = (node *)malloc(sizeof(node));
    if (!new_node) {
        return; // Handle memory allocation failure
    }
    
    new_node->filename = strdup(filename); // Duplicate the filename
    new_node->next = NULL;
    new_node->prev = l->tail;

    if (l->tail) {
        l->tail->next = new_node;
    } else {
        l->head = new_node; // If the list was empty, set head to the new node
    }
    
    l->tail = new_node; // Update tail to the new node
}

void remove_node(list *l, const char *filename) {
    node *current = l->head;
    
    while (current) {
        if (strcmp(current->filename, filename) == 0) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                l->head = current->next; // If it's the head node
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                l->tail = current->prev; // If it's the tail node
            }
            
            free(current->filename); // Free the duplicated filename
            free(current); // Free the node itself
            return;
        }
        current = current->next;
    }
}

void print_debug_list(const list *file_list) {
    if (file_list == NULL || file_list->head == NULL) {
        printf("Debug list is empty.\n");
        return;
    }

    node *current = file_list->head;
    while (current != NULL) {
        printf("File: %s\n", current->filename);
        current = current->next;
    }
}