// pokedex.h
// 
// This program was written by Zrx
// on 14 Nov 2019
// 
// Version 2.0.0: Release
// Version 2.0.1: Fix detail_pokemon comment to have 1 dp for height.

#include "pokemon.h"

#ifndef _POKEDEX_H_
#define _POKEDEX_H_

typedef struct pokedex *Pokedex;

#define DOES_NOT_EVOLVE (-64)

// Create a new Pokedex and return a pointer to it.
Pokedex new_pokedex(void);

void add_pokemon(Pokedex pokedex, Pokemon pokemon);

void detail_pokemon(Pokedex pokedex);

Pokemon get_current_pokemon(Pokedex pokedex);

void find_current_pokemon(Pokedex pokedex);

void print_pokemon(Pokedex pokedex);

void next_pokemon(Pokedex pokedex);

void prev_pokemon(Pokedex pokedex);

// Change the currently selected Pokemon in the Pokedex to be the
// Pokemon with the ID `id`.
//
// If there is no Pokemon with the ID `id`, this function does
// nothing.
void change_current_pokemon(Pokedex pokedex, int id);

// If there are no Pokemon in the Pokedex, this function does nothing.
void remove_pokemon(Pokedex pokedex);

// Destroy the given Pokedex and free all associated memory.
void destroy_pokedex(Pokedex pokedex);

void go_exploring(Pokedex pokedex);

// Return the total number of Pokemon in the Pokedex, whether or not
// they have been found.
int count_total_pokemon(Pokedex pokedex);

// Return the number of Pokemon in the Pokedex that have been found.
int count_found_pokemon(Pokedex pokedex);

void add_pokemon_evolution(Pokedex pokedex, int from_id, int to_id);

void show_evolutions(Pokedex pokedex);

int get_next_evolution(Pokedex pokedex);

Pokedex get_pokemon_of_type(Pokedex pokedex, pokemon_type type);

Pokedex get_found_pokemon(Pokedex pokedex);

Pokedex search_pokemon(Pokedex pokedex, char *text);

#endif //  _POKEDEX_H_
