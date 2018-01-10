#ifndef SNAKE_G_TILE_HPP
#define SNAKE_G_TILE_HPP


//--------------------------------------------------------------
// In-game tile
//--------------------------------------------------------------

#include <SDL2/SDL_rect.h>


struct Tile {
	enum Entity {
		NONE,

		SNAKE_HEAD,
		SNAKE_BODY,

		SIMPLE_FOOD,
		// ... other food types

		COUNT
	};

	int x, y;
	Entity entity;

	static int size;

	Tile() noexcept;
	Tile(int x, int y, Entity entity) noexcept;

	inline operator SDL_Rect() const noexcept {return {x, y, size, size};}
};


#endif // SNAKE_G_TILE_HPP
