// pokemon.h
//
// This program was written by Zrx
// on 14 Nov 2019
// 
// Version 2.0.0 Release

#ifndef _POKEMON_H_
#define _POKEMON_H_

typedef enum pokemon_type {
    INVALID_TYPE = -1,
    NONE_TYPE,
    NORMAL_TYPE,
    FIRE_TYPE,
    FIGHTING_TYPE,
    WATER_TYPE,
    FLYING_TYPE,
    GRASS_TYPE,
    POISON_TYPE,
    ELECTRIC_TYPE,
    GROUND_TYPE,
    PSYCHIC_TYPE,
    ROCK_TYPE,
    ICE_TYPE,
    BUG_TYPE,
    DRAGON_TYPE,
    GHOST_TYPE,
    DARK_TYPE,
    STEEL_TYPE,
    FAIRY_TYPE,
    MAX_TYPE
} pokemon_type;

typedef struct pokemon *Pokemon;


Pokemon new_pokemon(int pokemon_id, char *name, double height,
    double weight, pokemon_type type1, pokemon_type type2);

// Free the memory used for a given Pokemon.
void destroy_pokemon(Pokemon pokemon);

// Create a clone of the input Pokemon.
Pokemon clone_pokemon(Pokemon pokemon);

// Return the pokemon_id of a given Pokemon.
int pokemon_id(Pokemon pokemon);

// Return the name of a given Pokemon.
char *pokemon_name(Pokemon pokemon);

// Return the height of a given Pokemon.
double pokemon_height(Pokemon pokemon);

// Return the weight of a given Pokemon.
double pokemon_weight(Pokemon pokemon);

// Return the first type of a given Pokemon.
pokemon_type pokemon_first_type(Pokemon pokemon);

// Return the second type of a given Pokemon.
// If the Pokemon only has one type, returns NONE_TYPE.
pokemon_type pokemon_second_type(Pokemon pokemon);

// Check whether the specified "name" is a valid name for a Pokemon.
// Returns 1 if all the characters are valid Pokemon name characters.
// Returns 0 if not.
// The name must consist only of uppercase letters, lowercase letters,
// spaces (' ') and hyphens ('-').
int pokemon_valid_name(char *name);


// Return the pokemon_type value for the given "type_name".
//
// Example: pokemon_type_from_string("Water") returns WATER_TYPE.
//
// Returns INVALID_TYPE if "type_name" is not a valid Pokemon type.
pokemon_type pokemon_type_from_string(char *type_name);

// Return the string name of the given pokemon_type value.
//
// Example: pokemon_type_to_string(WATER_TYPE) returns "Water".
//
// The returned string has not been allocated with malloc.
// It should not be changed or freed by the caller.
const char *pokemon_type_to_string(pokemon_type type);

#endif // _POKEMON_H_
