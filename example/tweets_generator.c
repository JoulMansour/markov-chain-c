#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "markov_chain.h"

#define NUM_1 1
#define NUM_2 2
#define NUM_3 3
#define NUM_4 4
#define NUM_5 5
#define NUM_10 10
#define NUM_20 20
#define NUM_1000 1000


//Don't change the macros!
#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"

#define DELIMITERS " \n\t\r"

void print_function(const void* data) {
    printf("%s", (char*)data);
}

int comp_function(const void* data1, const void* data2) {
    const char* str1 = (const char*)data1;
    const char* str2 = (const char*)data2;
    return strcmp(str1, str2);
}

void free_function(void* data) {
    char* str = (char*)data;
    free(str);
}

void* copy_function(const void* data) {
    char* str = (char*)data;
    size_t len = strlen(str);
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, str);
    return copy;
}

bool is_last_word(const void* data) {
    const char *str = (const char*)data;
    size_t len = strlen(str);
    if (str[len - 1] == '.') {
        return true;
    }
    return false;
}



int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain) {
    char line[NUM_1000];
    int word_count = 0;
    MarkovNode *prev_node = NULL;
    while (fgets(line, NUM_1000, fp) != NULL &&
        word_count < words_to_read) {
        char *word = strtok(line, DELIMITERS);
        while (word != NULL && word_count < words_to_read) {
            Node *current_node = add_to_database(markov_chain, word);
            if (current_node == NULL) {
                return NUM_1;
            }
            if (prev_node != NULL) {
                if (add_node_to_frequency_list(prev_node,
                    current_node->data) != 0) {
                    return NUM_1;
                }
            }
            if (markov_chain->is_last(word)) {
                prev_node = NULL;
            } else {
                prev_node = current_node->data;
            }
            word_count++;
            word = strtok(NULL, DELIMITERS);
        }
        prev_node = NULL;
    }
    return 0;
}


MarkovChain *initialize_markov_chain() {
    MarkovChain *markov_chain = malloc(sizeof(MarkovChain));
    if (markov_chain == NULL) {
        return NULL;
    }
    markov_chain->database = malloc(sizeof(LinkedList));
    if (markov_chain->database == NULL) {
        free(markov_chain);
        return NULL;
    }
    markov_chain->database->first = NULL;
    markov_chain->database->last = NULL;
    markov_chain->database->size = 0;
    return markov_chain;
}


void generate_tweets(MarkovChain *markov_chain, int num_of_tweets) {
    for (int i = NUM_1; i <= num_of_tweets; i++) {
        printf("Tweet %d: ", i);
        MarkovNode *start_node = get_first_random_node(markov_chain);
        if (start_node != NULL) {
            generate_random_sequence(markov_chain, start_node, NUM_20);
        }
    }
}



int main(int argc, char *argv[]) {
    if (argc < NUM_4 || argc > NUM_5) {
        printf(NUM_ARGS_ERROR);
        return EXIT_FAILURE;
    }
    unsigned int seed = (unsigned int)strtoul(argv[NUM_1], NULL, NUM_10);
    int num_of_tweets = (int)strtol(argv[NUM_2], NULL, NUM_10);
    char *file_path = argv[NUM_3];
    int words_to_read;
    if (argc == NUM_5) {
        words_to_read = (int)strtol(argv[NUM_4], NULL, NUM_10);
    } else {
        words_to_read = -1;
    }
    if (words_to_read <= 0) {
        words_to_read = INT_MAX;
    }
    srand(seed);
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        printf(FILE_PATH_ERROR);
        return EXIT_FAILURE;
    }
    MarkovChain *markov_chain = initialize_markov_chain();
    if (markov_chain == NULL) {
        printf(ALLOCATION_ERROR_MESSAGE);
        fclose(fp);
        return EXIT_FAILURE;
    }
    markov_chain->print_f = print_function;
    markov_chain->comp_f = comp_function;
    markov_chain->free_data = free_function;
    markov_chain->copy_f = copy_function;
    markov_chain->is_last = is_last_word;
    if (fill_database(fp, words_to_read, markov_chain) != 0) {
        printf(ALLOCATION_ERROR_MESSAGE);
        free_database(&markov_chain);
        fclose(fp);
        return EXIT_FAILURE;
    }
    fclose(fp);
    generate_tweets(markov_chain, num_of_tweets);
    free_database(&markov_chain);
    return EXIT_SUCCESS;
}


