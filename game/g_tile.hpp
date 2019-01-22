#ifndef SNAKE_GAME_G_TILE_HPP
#define SNAKE_GAME_G_TILE_HPP


//--------------------------------------------------------------
// In-game tile
//--------------------------------------------------------------

#include <SDL2/SDL_rect.h>


/**
 * > Not each tile should know its position. That was a dumb idea
 * > 32bit int for each tile. First half for FLAGS, other 16 bits for snake-body-part-index,
 * since I want to keep the death effect.
 */


struct Tile {
	enum Entity {
		NONE,

		SNAKE_HEAD,
		SNAKE_BODY,

		SIMPLE_FOOD,
		// ... other food types

		COUNT
	};

	Entity entity;

	Tile() noexcept;
	Tile(Entity entity) noexcept;

	Tile(const Tile&) = default;
	Tile& operator=(const Tile&) = default;

	Tile(Tile&&) = default;
	Tile& operator=(Tile&&) = default;
};


#endif // SNAKE_GAME_G_TILE_HPP
