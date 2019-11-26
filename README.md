# Pokedex
Use a linked list to implement operations on the pokedex.


# QuickStart
    gcc main.c pokemon.c pokedex.c
    ./a.out

    ===========================[ Pokedex ]==========================
            Welcome to the Pokedex!  How can I help?
    ================================================================ 
    Enter command: ?
    ============================[ Help ]============================
      a [pokemon_id] [name] [height] [weight] [type1] [type2]
        Add a Pokemon to the Pokedex
      p
        Print all of the Pokemon in the Pokedex (in the order they were added)
      g
        Print currently selected Pokemon
      d
        Display details of the currently selected Pokemon
      >
        Move the cursor to the next Pokemon in the Pokedex
      <
        Move the cursor to the previous Pokemon in the Pokedex
      m [pokemon_id]
        Move the cursor to the Pokemon with the specified pokemon_id
      r
        Remove the current Pokemon from the Pokedex
      y
        Show Pokemon types
      x
        Go exploring for Pokemon
      f
        Set the current Pokemon to be found
      c
        Print out the count of Pokemon who have been found
      t
        Print out the total count of Pokemon in the Pokedex
      e [pokemon_A] [pokemon_B]
        Add an evolution from Pokemon A to Pokemon B
      s
        Show evolutions of the currently selected Pokemon
      n
        Show next evolution of current selected Pokemon
      F
         Create a new Pokedex containing Pokemon that have previously been found
      S [string]
         Create a new Pokedex containing Pokemon that have the specified string in their name
      T [type]
         Create a new Pokedex containing Pokemon that have the specified type
      q
        Quit
      ?
        Show help
    ================================================================
