#include "markov_chain.h"

#include <string.h>

#define NUM_1 1
/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}

Node *get_node_from_database(MarkovChain *markov_chain, void *data_ptr) {
    if (markov_chain == NULL || markov_chain->database == NULL) {
        return NULL;
    }
    Node *current_node = markov_chain->database->first;
    while (current_node != NULL) {
        MarkovNode *markov_data = current_node->data;
        if (markov_chain->comp_f(markov_data->data, data_ptr) == 0) {
            return current_node;
        }
        current_node = current_node->next;
    }
    return NULL;
}


Node* add_to_database(MarkovChain *markov_chain, void *data_ptr) {
    Node *node = get_node_from_database(markov_chain, data_ptr);
    if (node != NULL) {
        return node;
    }
    MarkovNode* new_markov_node = malloc(sizeof(MarkovNode));
    if (new_markov_node == NULL) {
        return NULL;
    }
    new_markov_node->data = markov_chain->copy_f(data_ptr);
    if (new_markov_node->data == NULL) {
        markov_chain->free_data(new_markov_node->data);
        return NULL;
    }
    new_markov_node->frequency_list = NULL;
    new_markov_node->frequency_size = 0;
    if (add(markov_chain->database, new_markov_node) != 0) {
        return NULL;
    }
    return markov_chain->database->last;
}


int add_node_to_frequency_list(MarkovNode *first_node,
    MarkovNode *second_node) {
    if(!first_node->frequency_list) {
        first_node->frequency_list = malloc(sizeof(MarkovNodeFrequency));
        if(first_node->frequency_list == NULL) {
            printf(ALLOCATION_ERROR_MESSAGE);
            return EXIT_FAILURE;
        }
        first_node->frequency_size = NUM_1;
        first_node->frequency_list[0].markov_node = second_node;
        first_node->frequency_list[0].frequency= NUM_1;
        return EXIT_SUCCESS;
    }
    MarkovNodeFrequency *current_frequency = first_node->frequency_list;
    for(int i=0; i<first_node->frequency_size; i++) {
        if(current_frequency[i].markov_node == second_node) {
            current_frequency[i].frequency++;
            return EXIT_SUCCESS;
        }
    }
    first_node->frequency_list = realloc(first_node->frequency_list,
        sizeof(MarkovNodeFrequency) * (first_node->frequency_size + NUM_1));
    if(first_node->frequency_list == NULL) {
        printf(ALLOCATION_ERROR_MESSAGE);
        return EXIT_FAILURE;
    }
    first_node->frequency_list[first_node->frequency_size].markov_node \
    = second_node;
    first_node->frequency_list[first_node->frequency_size].frequency = 1;
    first_node->frequency_size += 1;
    return 0;
}


void free_database(MarkovChain ** ptr_chain) {
    if (ptr_chain == NULL || *ptr_chain == NULL) {
        return;
    }
    MarkovChain *markov_chain = *ptr_chain;
    if (markov_chain->database != NULL) {
        Node *current_node = markov_chain->database->first;
        while (current_node != NULL) {
            Node *next_node = current_node->next;
            MarkovNode *markov_node = current_node->data;
            if (markov_node != NULL) {
                if(markov_node->frequency_list) {
                    free(markov_node->frequency_list);
                }
                markov_chain->free_data(markov_node->data);
                free(markov_node);
            }
            free(current_node);
            current_node = next_node;
        }
        free(markov_chain->database);
    }
    free(markov_chain);
    *ptr_chain = NULL;
}


MarkovNode *get_first_random_node(MarkovChain *markov_chain) {
    int i = get_random_number(markov_chain->database->size);
    Node *node = markov_chain->database->first;
    for (int j = 0; j < i; j++) {
        node = node->next;
    }
    if (markov_chain->is_last(node->data->data) ||
        node->data->frequency_list == NULL) {
        return get_first_random_node(markov_chain);
    }
    return node->data;
}


MarkovNode* get_next_random_node(MarkovNode *cur_markov_node) {
    int total_frequency = 0;
    for(int i = 0; i < cur_markov_node->frequency_size; i++) {
        total_frequency += cur_markov_node->frequency_list[i].frequency;
    }
    int random_number = get_random_number(total_frequency);
    for(int i = 0; i < cur_markov_node->frequency_size; i++) {
        random_number -= cur_markov_node->frequency_list[i].frequency;
        if(random_number < 0) {
            return cur_markov_node->frequency_list[i].markov_node;
        }
    }
    return NULL;
}


void generate_random_sequence(MarkovChain *markov_chain,
                              MarkovNode *first_node, int max_length) {
    MarkovNode *current_node = first_node;
    int word_count = 0;
    while (current_node != NULL && word_count < max_length) {
        markov_chain->print_f(current_node->data);
        word_count++;
        if (current_node->frequency_list == NULL) {
            break;
        }
        if (word_count < max_length) {
            printf(" ");
        }
        current_node = get_next_random_node(current_node);
    }
    printf("\n");
}


