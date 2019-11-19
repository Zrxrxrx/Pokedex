// pokemon.c
//
// This program was written by Zrx
// on 14 Nov 2019
// 
// Version 2.0.0: Release

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pokemon.h"

const static char *types[] = {
    [NONE_TYPE]     = "None",
    [NORMAL_TYPE]   = "Normal",
    [FIRE_TYPE]     = "Fire",
    [FIGHTING_TYPE] = "Fighting",
    [WATER_TYPE]    = "Water",
    [FLYING_TYPE]   = "Flying",
    [GRASS_TYPE]    = "Grass",
    [POISON_TYPE]   = "Poison",
    [ELECTRIC_TYPE] = "Electric",
    [GROUND_TYPE]   = "Ground",
    [PSYCHIC_TYPE]  = "Psychic",
    [ROCK_TYPE]     = "Rock",
    [ICE_TYPE]      = "Ice",
    [BUG_TYPE]      = "Bug",
    [DRAGON_TYPE]   = "Dragon",
    [GHOST_TYPE]    = "Ghost",
    [DARK_TYPE]     = "Dark",
    [STEEL_TYPE]    = "Steel",
    [FAIRY_TYPE]    = "Fairy",
};

#define POKEMON_MAGIC_NUMBER 0xDEADBEEF

struct pokemon {
    int          magic_number;
    int          pokemon_id;
    char         *name;
    double       height;
    double       weight;
    pokemon_type type1;
    pokemon_type type2;
};

static int valid_character(int c);
static int valid_pokemon_type(pokemon_type type);
static void check_valid_pokemon(Pokemon pokemon, char *function_name);
static void die(char *function_name, char *message);
static char *check_address_is_heap_pointer(void *p, size_t size);

Pokemon new_pokemon(int pokemon_id, char *name, double height,
    double weight, pokemon_type type1, pokemon_type type2) {

    if (pokemon_id < 0) {
        die("new_pokemon", "invalid pokemon_id");
    }

    if (!valid_pokemon_type(type1)) {
        die("new_pokemon", "type1 is invalid");
    }

    if (!valid_pokemon_type(type2)) {
        die("new_pokemon", "type2 is invalid");
    }

    if (type1 == NONE_TYPE) {
        die("new_pokemon", "type1 is NONE_TYPE");
    }

    if (type1 == type2) {
        die("new_pokemon", "type1 and type2 must be different");
    }

    Pokemon new_pokemon = malloc(sizeof(struct pokemon));
    assert(new_pokemon != NULL);

    new_pokemon->magic_number = POKEMON_MAGIC_NUMBER;
    new_pokemon->pokemon_id = pokemon_id;
    new_pokemon->name = strdup(name);
    assert(name != NULL);
    new_pokemon->height = height;
    new_pokemon->weight = weight;
    new_pokemon->type1 = type1;
    new_pokemon->type2 = type2;
    return new_pokemon;
}

// Destroy the specified `pokemon`.
void destroy_pokemon(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "destroy_pokemon");
    free(pokemon->name);
    free(pokemon);
}

// Return the pokemon_id of the specified `pokemon`.
int pokemon_id(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "pokemon_id");
    return pokemon->pokemon_id;
}

// Return the pokemon_name of the specified `pokemon`.
char *pokemon_name(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "pokemon_name");
    return pokemon->name;
}

// Return the pokemon_height of the specified `pokemon`.
double pokemon_height(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "pokemon_height");
    return pokemon->height;
}

// Return the pokemon_weight of the specified `pokemon`.
double pokemon_weight(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "pokemon_weight");
    return pokemon->weight;
}

// Return the first type of the specified `pokemon`.
pokemon_type pokemon_first_type(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "pokemon_first_type");
    return pokemon->type1;
}

// Return the second type of the specified `pokemon`.
pokemon_type pokemon_second_type(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "pokemon_second_type");
    return pokemon->type2;
}

// Return a clone of the specified `pokemon`.
Pokemon clone_pokemon(Pokemon pokemon) {
    check_valid_pokemon(pokemon, "clone_pokemon");
    return new_pokemon(
        pokemon->pokemon_id,
        pokemon->name,
        pokemon->height,
        pokemon->weight,
        pokemon->type1,
        pokemon->type2
    );
}

// Check whether `name` is a valid name for a Pokemon.
// Valid names consist of letters, spaces, and dashes.
int pokemon_valid_name(char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!valid_character(name[i])) {
            return 0;
        }
    }
    return 1;
}


static int valid_character(int c) {
    return (c == ' ' ||
            c == '-' ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z')
    );
}

pokemon_type pokemon_type_from_string(char *str) {
    for (int i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
        if (strcasecmp(types[i], str) == 0) {
            return (pokemon_type) i;
        }

    }
    return INVALID_TYPE;
}

// Convert a pokemon_type into its corresponding string.
// For example: pokemon_type_to_string(FIRE_TYPE) would return "Fire".
const char *pokemon_type_to_string(pokemon_type type) {
    assert(valid_pokemon_type(type));
    return types[type];
}

// Check whether `type` is a valid pokemon_type.
static int valid_pokemon_type(pokemon_type type) {
    return type > INVALID_TYPE && type < MAX_TYPE;
}

static void check_valid_pokemon(Pokemon pokemon, char *function_name) {
    if (pokemon == NULL) {
        die(function_name, "pokemon is NULL");
    }

    char *error = check_address_is_heap_pointer(pokemon, sizeof *pokemon);
    if (error != NULL) {
        die(function_name, error);
    }

    if (pokemon->magic_number != POKEMON_MAGIC_NUMBER) {
        die(function_name, "invalid pokemon");
    }
}

static void die(char *function_name, char *message) {
    fprintf(stderr, "%s: %s\n", function_name, message);
    exit(1);
}


#ifdef __has_feature
// detect clang address-sanitizer
#if __has_feature(address_sanitizer)
// clang address-sanitizer
#define HAVE_ASAN
#endif
#endif

// detect gcc address-sanitizer
#ifdef __SANITIZE_ADDRESS__
#define HAVE_ASAN
#endif

#ifdef HAVE_ASAN
#include <sanitizer/asan_interface.h>
#include <stdint.h>
#include <string.h>
#endif

// return NULL if p is valid pointer to a malloc'ed region of size `size`
// otherwise return string describing pointer

static char *check_address_is_heap_pointer(void *p, size_t size) {
#ifdef HAVE_ASAN
    if (p == NULL) {
        return NULL;
    }
    if ((sizeof(p) == 8 && (uintptr_t)p == 0xbebebebebebebebe)
            || (sizeof(p) == 4 && (uintptr_t)p == 0xbebebebe)) {
        return "pokemon is uninitialized";
    }
    if (__asan_region_is_poisoned(p, size)) {
        return "pokemon is invalid";
    }
    char name[8]; // unused but required by __asan_locate_address
    void *region_address;
    size_t region_size;
    if (strcmp(__asan_locate_address(p, name, sizeof name,
            &region_address, &region_size), "heap") != 0) {
        return "pokemon is invalid (not from malloc)";
    }
#endif
    return NULL;
}
