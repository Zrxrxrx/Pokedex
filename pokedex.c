// pokedex.c
//
// This program was written by Zrx
// on 14 Nov 2019
//
// Version 5.0.6: debug

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pokedex.h"

#define MAX_STRING_LENGTH 256
#define MAX_TYPE 19
#define NOT_FOUND 0
#define FOUND 1
#define NONE_TYPE 0
#define TURE 1
#define FALSE 0

struct pokedex {
    struct pokenode *head;
    struct pokenode *end;
    struct pokenode *current;
    struct pokedata *data;
    struct pokedex *next;
};

struct pokenode {
    Pokemon pokemon;
    struct pokenode *next;
    struct pokenode *prev;
    struct pokenode *evolve;
    int             status;
};

// This struct is used to store the data of pokedex
// including total Pokemon number, found Pokemon number
// and a type_record array in the pokedex. 
struct pokedata {
    int total;
    int found_nums;
    int type_record[MAX_TYPE];
};


// Creates a new pokenode struct and returns a pointer to it.
static struct pokenode *new_pokenode(Pokemon pokemon);

// Return the string length.
static int get_string_length(char *string);

// Print the Pokemond name which is not found.
// for example #001:********
static void print_unfound_name(int length);

// Store type into array.
static void type_record(int *array, int type);

// Set all element of array to 0.
static void clean_array(int *array);

// Print the type string in the type_array.
static void print_type_array(int *array);

// Return whether the type is stored before
// if not, it will store current type into array.
static int type_exploring(int *array, int type);

// This function is used to debug.
static void debug_array(int *array);

// Helper Function : return the node address, 
// which has the parameter ID.
static struct pokenode *set_evolution(Pokedex pokedex, int id);

// Print the evolution of current Pokemon.
static void print_evolution(struct pokenode *node);

// Insert new pokenode into the end of Pokedex.
static void insert_end_node(struct pokenode *new_node, Pokedex pokedex);

// Insert new pokenode into the head of Pokedex.
static void insert_head_node(struct pokenode *new_node, Pokedex pokedex);

// Insert new pokenode before the node insert_after in current Pokedex.
static void insert_between_node(struct pokenode *new_node, struct pokenode *insert_after, Pokedex pokedex);

// Helper function : clone pokemon and create a node, 
// and add to subPokedex.
static void transfer_pokemon(struct pokenode *node, Pokedex pokedex);

// Insert pokenode in order.
static void insert_order_pokemon(struct pokenode *node, Pokedex pokedex);

// Set all pokemons are found in current pokedex.
static void set_all_found(Pokedex pokedex);

// Helper function
// Determine if the current name contains text.
static int compare_name(char *text, char *curr_name);

// Make uppercase character lowercase.
static char to_lower_case(char text);

// Search for Pokemon with this ID in pokedex.
static void check_exist_pokemon(int id, Pokedex pokedex);

// Helper function
// When Pokemon is removed, 
// This function will adjuest found numbers.
static void remove_count_number(Pokedex pokedex, struct pokenode *n);

// Creates a new Pokedex, and returns a pointer to it.
Pokedex new_pokedex(void) {
    // Malloc memory for a new Pokedex, and check that the memory
    // allocation succeeded.
    Pokedex pokedex = malloc(sizeof(struct pokedex));
    assert(pokedex != NULL);

    // Set the head of the linked list to be NULL.
    // (i.e. set the Pokedex to be empty)
    pokedex->head = NULL;
    pokedex->end = NULL;
    pokedex->current = NULL;
    pokedex->next = NULL;
    
    struct pokedata *data = malloc(sizeof(struct pokedata));
    assert(data != NULL);
    pokedex->data = data;
    pokedex->data->total = 0;
    pokedex->data->found_nums = 0;
    clean_array(pokedex->data->type_record);

    return pokedex;
}

// Create a new pokenode struct and returns a pointer to it.
static struct pokenode *new_pokenode(Pokemon pokemon) {

    struct pokenode *new = malloc(sizeof(struct pokenode));
    assert(new != NULL);
    new->pokemon = pokemon;
    new->next = NULL;
    new->prev = NULL;
    new->evolve = NULL;
    new->status = NOT_FOUND;

    return new;
}

// Add a new Pokemon to the Pokedex.
void add_pokemon(Pokedex pokedex, Pokemon pokemon) {

    int add_id = pokemon_id(pokemon);
    check_exist_pokemon(add_id, pokedex);

    struct pokenode *new_node = new_pokenode(pokemon);

    if (pokedex->end == NULL) {
        // pokenode is empty

        pokedex->head = new_node;
        pokedex->current = new_node;
        pokedex->end = new_node;
        pokedex->data->total++;

    } else {
        // pokenode has more than one node

        insert_end_node(new_node, pokedex);

    }

}

// Print out the details of the currently selected Pokemon.
void detail_pokemon(Pokedex pokedex) {
    
    if (pokedex->head == NULL) {
        return;
    }
    
    struct pokenode *curr= pokedex->current;

    int name_length = get_string_length(pokemon_name(curr->pokemon));

    printf("ID: %03d\n", pokemon_id(curr->pokemon));
    if (curr->status == NOT_FOUND) {
        printf("Name: ");
        print_unfound_name(name_length);
        printf("Height: --\n");
        printf("Weight: --\n");
        printf("Type: --\n");
    } else {
        printf("Name: %s\n", pokemon_name(curr->pokemon));
        printf("Height: %.1lfm\n", pokemon_height(curr->pokemon));
        printf("Weight: %.1lfkg\n", pokemon_weight(curr->pokemon));

        int type1 = pokemon_first_type(curr->pokemon);
        int type2 = pokemon_second_type(curr->pokemon);

        const char *type1_string = pokemon_type_to_string(type1);
        const char *type2_string = pokemon_type_to_string(type2);

        printf("Type: %s ", type1_string);

        if (type2 != 0) {
            printf("%s\n", type2_string);
        } else {
            printf("\n");
        }

    }
    
}

// Return the currently selected Pokemon.
Pokemon get_current_pokemon(Pokedex pokedex) {

    if (pokedex->current == NULL) {
        return NULL;
    }

    return pokedex->current->pokemon;

}

// Sets the currently selected Pokemon to be 'found'.
void find_current_pokemon(Pokedex pokedex) {

    if (pokedex->current->status == NOT_FOUND) {
        pokedex->data->found_nums++;
    }
    pokedex->current->status = FOUND;

}

// Print out all of the Pokemon in the Pokedex.
void print_pokemon(Pokedex pokedex) {

    struct pokenode *curr = pokedex->head;
    

    while (curr != NULL) {
        int name_length = get_string_length(pokemon_name(curr->pokemon));
        if (curr->pokemon == get_current_pokemon(pokedex)) {
            printf("--> ");
            printf("#%03d: ", pokemon_id(curr->pokemon));
            if (curr->status == NOT_FOUND) {
                print_unfound_name(name_length);
            } else {
                printf("%s\n", pokemon_name(curr->pokemon));
            }
        } else {
            if (curr->status == NOT_FOUND) {
                printf("    #%03d: ", pokemon_id(curr->pokemon));
                print_unfound_name(name_length);
            } else {
                printf(
                    "    #%03d: %s\n", 
                    pokemon_id(curr->pokemon), 
                    pokemon_name(curr->pokemon)
                );
            }
        }
        curr = curr->next;
    }

}

// Change the currently selected Pokemon to be the next Pokemon in the Pokedex.
void next_pokemon(Pokedex pokedex) {

    if (pokedex->current == NULL) {
        return;
    }

    struct pokenode *curr_node = pokedex->current;

    if (curr_node->next != NULL) {

        curr_node = curr_node->next;
        pokedex->current = curr_node;

    }

}

// Change the currently selected Pokemon to be the previous Pokemon in the Pokedex.
void prev_pokemon(Pokedex pokedex) {

    if (pokedex->current == NULL) {
        return;
    }

    struct pokenode *curr_node = pokedex->current;

    if (curr_node->prev != NULL) {

        curr_node = curr_node->prev;
        pokedex->current = curr_node;

    }

}

// Change the currently selected Pokemon to be the Pokemon with the ID `id`.
void change_current_pokemon(Pokedex pokedex, int id) {

    struct pokenode *curr_node = pokedex->head;
    while (curr_node != NULL) {
        if (pokemon_id(curr_node->pokemon) == id) {
            pokedex->current = curr_node;
        }
        curr_node = curr_node->next;
    }
    
}

// Remove the currently selected Pokemon from the Pokedex.
void remove_pokemon(Pokedex pokedex) {

    if (pokedex->end == NULL) {
        // pokenode is empty
        return;
    }

    struct pokenode *remove_node = pokedex->current;
    struct pokenode *end_node = pokedex->end;

    if (remove_node->next == NULL && remove_node->prev == NULL) {
        // pokendoe has only one node
        remove_count_number(pokedex, remove_node);
        destroy_pokemon(remove_node->pokemon);
        free(remove_node);
        pokedex->end = NULL;
        pokedex->head = NULL;
        pokedex->current = NULL;
        pokedex->data->total--;
        return;
    }


    if (remove_node == pokedex->head) {
        // remove head node
        pokedex->head = remove_node->next;
        pokedex->head->prev = NULL;
        pokedex->current = pokedex->head;

    } else if (remove_node == end_node) {
        // remove end node
        end_node = end_node->prev;
        end_node->next = NULL;
        pokedex->current = end_node;
        pokedex->end = end_node;

    } else {
        // remove intermediate node
        struct pokenode *after_node = remove_node->next;
        struct pokenode *previous_node = remove_node->prev;
        previous_node->next = after_node;
        after_node->prev = previous_node;
        pokedex->current = after_node;

    }
    remove_count_number(pokedex, remove_node);
    destroy_pokemon(remove_node->pokemon);
    free(remove_node);
    pokedex->data->total--;
}

// Destroy the given Pokedex and free all associated memory.
void destroy_pokedex(Pokedex pokedex) {

    struct pokenode *curr = pokedex->head;

    while (curr != NULL) {

        struct pokenode *remove = curr;
        curr = curr->next;
        destroy_pokemon(remove->pokemon);
        free(remove);

    }

    free(pokedex->data);
    free(pokedex);
    pokedex = NULL;

}

// Print out all of the different types of Pokemon in the Pokedex.
void show_types(Pokedex pokedex) {

    struct pokenode *curr = pokedex->head;
    
    while (curr != NULL) {

        int type1 = pokemon_first_type(curr->pokemon);
        int type2 = pokemon_second_type(curr->pokemon);

        type_record(pokedex->data->type_record,type1);
        type_record(pokedex->data->type_record,type2);

        curr = curr->next;
    }
    print_type_array(pokedex->data->type_record);
    clean_array(pokedex->data->type_record);

}

// Set the first not-yet-found Pokemon of each type to be found.
void go_exploring(Pokedex pokedex) {

    struct pokenode *curr = pokedex->head;
    struct pokenode *origin_current = pokedex->current;

    while (curr != NULL) {
        pokedex->current = curr;
        if (curr->status == NOT_FOUND) {

            int type1 = pokemon_first_type(curr->pokemon);
            int type2 = pokemon_second_type(curr->pokemon);


            int type1_exploring = type_exploring(pokedex->data->type_record, type1);
            int type2_exploring = type_exploring(pokedex->data->type_record, type2);

            if (type1_exploring == TURE || type2_exploring == TURE) {
                find_current_pokemon(pokedex);
            }

        }

        curr = curr->next;
    }
    pokedex->current = origin_current;
    clean_array(pokedex->data->type_record);

}

// Return the total number of Pokemon in the Pokedex.
int count_total_pokemon(Pokedex pokedex) {

    return pokedex->data->total;

}

// Return the number of Pokemon in the Pokedex that have been found.
int count_found_pokemon(Pokedex pokedex) {

    return pokedex->data->found_nums;

}

// Add the information that the Pokemon with the ID `from_id` can
// evolve into the Pokemon with the ID `to_id`.
void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id) {
    struct pokenode *from = set_evolution(pokedex, from_id);
    struct pokenode *to = set_evolution(pokedex, to_id);
    if (from == NULL || to == NULL || from == to) {
        fprintf(stderr, "Invalid Pokemon Evolution!\n");
        exit(1);
    }

    from->evolve = to;

}

// Show the evolutions of the currently selected Pokemon.
void show_evolutions(Pokedex pokedex) {

    struct pokenode *curr = pokedex->current;

    if (curr == NULL) {
        return;
    }

    while (curr != NULL) {

        print_evolution(curr);

        curr = curr->evolve;
    }
    printf("\n");

}

// Return the pokemon_id of the Pokemon that the currently selected
// Pokemon evolves into.
int get_next_evolution(Pokedex pokedex) {
    struct pokenode *curr = pokedex->current;
    if (curr == NULL) {
        fprintf(stderr, "%s: The Pokedex is empty!\n", __FILE__);
        exit(1);
    }


    if (curr->evolve != NULL) {
        int id = pokemon_id(curr->evolve->pokemon);
        return id;
    } else {
        return DOES_NOT_EVOLVE;
    }

}

// Create a new Pokedex which contains only the Pokemon of a specified
// type from the original Pokedex.
Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type) {
    Pokedex type_pokedex = new_pokedex();
    pokedex->next = type_pokedex;

    struct pokenode *curr = pokedex->head;

    while (curr != NULL) {

        if (curr->status == FOUND) {

            int type1 = pokemon_first_type(curr->pokemon);
            int type2 = pokemon_second_type(curr->pokemon);

            if (type1 == type || type2 == type) {
                
                transfer_pokemon(curr, pokedex);

            }
        }

        curr = curr->next;
    }

    pokedex = pokedex->next;
    pokedex->current = pokedex->head;

    return pokedex;

}

// Create a new Pokedex which contains only the Pokemon that have
// previously been 'found' from the original Pokedex.
Pokedex get_found_pokemon(Pokedex pokedex) {
    Pokedex found_pokedex = new_pokedex();
    pokedex->next = found_pokedex;

    struct pokenode *curr = pokedex->head;

    while (curr != NULL) {

        if (curr->status == FOUND) {
            insert_order_pokemon(curr, pokedex->next);
        }

        curr = curr->next;
    }

    pokedex = pokedex->next;
    set_all_found(pokedex);
    pokedex->current = pokedex->head;

    return pokedex;

}

// Create a new Pokedex containing only the Pokemon from the original
// Pokedex which have the given string appearing in its name.
Pokedex search_pokemon(Pokedex pokedex, char *text) {
    Pokedex search_pokedex = new_pokedex();
    pokedex->next = search_pokedex;

    struct pokenode *curr = pokedex->head;

    while (curr != NULL) {
        char *curr_name = pokemon_name(curr->pokemon);
        
        if ((compare_name(text, curr_name) == TURE) && curr->status == FOUND) {
            transfer_pokemon(curr, pokedex);
        }

        curr = curr->next;
    }


    pokedex = pokedex->next;
    pokedex->current = pokedex->head;
    return pokedex;

}

// Return the current string length, 
// excluding '\0' characters
static int get_string_length(char *string) {
    int i = 0;
    int counter = 0;
    while (i < MAX_STRING_LENGTH) {
        if (string[i] != '\0') {
            counter++;
        } else {
            return counter;
        }
        i++;
    }
    return 0;
}

// Print the '*' of the length of the Pokemon name
static void print_unfound_name(int length) {
    int i = 0;
    while (i < length) {
        printf("*");
        i++;
    }
    printf("\n");
}

// This function checks if the current array has this type, 
// and if it does, it returns. 
// If it does not exist, 
// this type will be recorded the type into the array.
static void type_record(int *array, int type) {
    if (type == 0) {
        return;
    }

    int i = 0;
    while (i < MAX_TYPE) {
        if (type == array[i]) {
            return;
        }
        i++;
    }

    int j = 0;
    while (j < MAX_TYPE && array[j] != NONE_TYPE) {
        j++;
    }

    array[j] = type;

}

// This function will change all elements 
// in the array to NONE_TYPE (0).
static void clean_array(int *array) {
    int i = 0;
    while (i < MAX_TYPE) {
        array[i] = NONE_TYPE;
        i++;
    }
}

// This function will iterate over the array 
// and print the type of string, except NONE_TYPE
static void print_type_array(int *array) {
    int i = 0;
    while (i < MAX_TYPE) {

        if (array[i] != NONE_TYPE) {
            const char *type_string = pokemon_type_to_string(array[i]);
            printf("%s\n", type_string);
        }

        i++;
    }
}

// This function checks if the current array has this type, 
// and if it does, it return TURE. 
// If it does not exist, 
// this type will be recorded the type into the array
// and return FLASE.
static int type_exploring(int *array, int type) {
    
    int i = 0;
    while (i < MAX_TYPE) {
        if (type == array[i]) {
            return FALSE;
        }
        i++;
    }

    int j = 0;
    while (j < MAX_TYPE && array[j] != NONE_TYPE) {
        j++;
    }

    array[j] = type;

    return TURE;
    
}

// This function is used to debug array
static void debug_array(int *array) {
    int i = 0;
    while (i < MAX_TYPE) {
        printf("%d ", array[i]);
        i++;
    }
    printf("\n");
}

// This function will return a pokenode address with the same id,
// if the same ID does not exist it will return a null address.
static struct pokenode *set_evolution(Pokedex pokedex, int id) {
    struct pokenode *curr = pokedex->head;

    while (curr != NULL) {
        if (pokemon_id(curr->pokemon) == id)
            return curr;

        curr = curr->next;
    }

    return NULL;
}

// Traverse the evolution of Pokemon and print it out.
// If the Pokemond is found, 
// it will print Pokemon id, name and type,
// otherwise, it only print #ID ???? [????]
static void print_evolution(struct pokenode *node) {
    printf("#%03d ",pokemon_id(node->pokemon));
    if (node->status == FOUND) {

        printf("%s ", pokemon_name(node->pokemon));

        int type1 = pokemon_first_type(node->pokemon);
        int type2 = pokemon_second_type(node->pokemon);

        const char *type1_string = pokemon_type_to_string(type1);
        const char *type2_string = pokemon_type_to_string(type2);

        printf("[%s", type1_string);

        if (type2 != 0) {
            printf(" %s] ", type2_string);
        } else {
            printf("] ");
        }
    } else {
        printf("???? [????] ");
        
    }

    if (node->evolve != NULL) {
        printf("--> ");
    }

}

// Insert new_node in the end of Pokedex.
static void insert_end_node(struct pokenode *new_node, Pokedex pokedex) {
    new_node->prev = pokedex->end;
    pokedex->end->next = new_node;
    pokedex->end = new_node;
    pokedex->data->total++;

}

// Insert new_node in the head of Pokedex.
static void insert_head_node(struct pokenode *new_node, Pokedex pokedex) {
    pokedex->head->prev = new_node;
    new_node->next = pokedex->head;
    pokedex->head = new_node;
    pokedex->data->total++;

}

// Insert new_node before the node insert_after.
static void insert_between_node(
    struct pokenode *new_node, 
    struct pokenode *insert_after, 
    Pokedex pokedex
) {

    new_node->prev = insert_after->prev;
    new_node->next = insert_after;
    insert_after->prev->next = new_node;
    insert_after->prev = new_node;
    pokedex->data->total++;

}

// Clone pokemon and create a node, 
// and add to subPokedex.
static void transfer_pokemon(struct pokenode *node, Pokedex pokedex) {
    Pokemon copy_pokemon = node->pokemon;
    add_pokemon(pokedex->next,clone_pokemon(copy_pokemon));
    pokedex->next->end->status = FOUND;
    pokedex->next->data->found_nums++;
}

// Insert pokenode in order.
// If Pokedex is empty, it will insert on the head.
// If Pokedex is not empty, 
// it will determine the order of the ID 
// to decide whether to 
// insert it in front of or behind the current node.
static void insert_order_pokemon(struct pokenode *node, Pokedex pokedex) {

    if (pokedex->head == NULL) {
        add_pokemon(pokedex, clone_pokemon(node->pokemon));
        return;
    }

    struct pokenode *new_node = new_pokenode(clone_pokemon(node->pokemon));
    int insert_id = pokemon_id(new_node->pokemon);

    if (pokedex->data->total == 1) {
        if (insert_id > pokemon_id(pokedex->head->pokemon)) {
            insert_end_node(new_node, pokedex);
        } else {
            insert_head_node(new_node, pokedex);
        }
        return;
    }

    if (pokedex->data->total > 1) {
        struct pokenode *insert = pokedex->head;
        while (insert != NULL && insert_id > pokemon_id(insert->pokemon)) {

            insert = insert->next;
        }
        if (insert == NULL) {
            insert_end_node(new_node, pokedex);
            return;
        } else if (insert->prev == NULL) {
            insert_head_node(new_node, pokedex);
            return;
        }

        insert_between_node(new_node, insert, pokedex);

    }

}

// Set all pokemons are found in current pokedex.
static void set_all_found(Pokedex pokedex) {
    struct pokenode *curr = pokedex->head;
    struct pokenode *origin_curr = pokedex->current;
    while (curr != NULL) {
        pokedex->current = curr;
        find_current_pokemon(pokedex);
        curr = curr->next;
    }
    pokedex->current = origin_curr;
}

// Determine if the current name contains text.
// if curr_name include the text
// it will return TURE;
// ortherwise it will return FALSE.
static int compare_name(char *text, char *curr_name) {

    int text_length = get_string_length(text);
    int curr_name_length = get_string_length(curr_name);

    int i = 0;
    int j = 0;
    while (i < curr_name_length) {
        char lower_curr_name = to_lower_case(curr_name[i]);
        char lower_text = to_lower_case(text[j]);

        if (lower_curr_name == lower_text) {
            j++;

            if (j >= text_length) {
                return TURE;
            }

        } else {

            j = 0;
            if (to_lower_case(text[j]) == to_lower_case(curr_name[i])) {
                j++;
            }

        }

        i++;
    }

    return FALSE;
}

// Make uppercase character lowercase.
static char to_lower_case(char text) {

    if (text >= 'A' && text <= 'Z') {
        return text + 'a' -'A';
    }
    return text;
    
}

// Search for Pokemon with this ID in pokedex.
// if the Pokemond will this id is exist,
// it will print the error information 
// and exit the program
static void check_exist_pokemon(int id, Pokedex pokedex) {
    struct pokenode *curr = pokedex->head;
    while (curr != NULL) {
        if (id == pokemon_id(curr->pokemon)) {
            fprintf(
                stderr, 
                "%s: There's already a Pokemon with pokemon_id %d!\n", 
                __FILE__, id
            );
            exit(1);
        }
        curr = curr->next;
    }
}

// When Pokemon is removed, 
// This function will adjuest found numbers.
static void remove_count_number(Pokedex pokedex, struct pokenode *n) {
    if (n->status == FOUND) {
        pokedex->data->found_nums--;
    }
}

