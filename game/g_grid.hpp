#ifndef SNAKE_GAME_G_GRID_HPP
#define SNAKE_GAME_G_GRID_HPP


//--------------------------------------------------------------
// In-game tile
//--------------------------------------------------------------

#include "g_tile.hpp"
#include "../core/core_texture.hpp"
#include <SDL2/SDL_rect.h>


constexpr int TILES_IN_ROW = 30;


namespace ui {

	extern core::Texman* gTexman;

	extern core::texindex txSnake;
	extern core::texindex txSimpleFood;
	extern core::texindex txHelpBackground;

	extern int speedLevel;
	extern SDL_Rect playArea;

	extern int tileSize;

} // namespace ui


enum class State {
	PLAYING,
	PAUSED,
	OVER,
	WON,
	MENU,
	QUIT
};


enum class Direction {
	NONE	= 0x0,	// 0000

	UP		= 0x1,	// 0001
	DOWN	= 0x2,	// 0010
	LEFT	= 0x4,	// 0100
	RIGHT	= 0x8	// 1000
};


class Grid {
public:
	// statics constants
	static const int MAX				= TILES_IN_ROW * TILES_IN_ROW;
	static const int INIT_SNAKE_SIZE	= 4;

	// interface
	Grid();

	State generateFood();

	void draw() const;
	void handleKeyboard();

	State advanceState();
	bool collapseSnakeTowardsItsMiddle();

	inline uint getScore() noexcept {
		return score;
	}

private:
	inline void incScore() noexcept {
		score += ui::speedLevel;
	}

	// tiles & snake
	Tile grid[TILES_IN_ROW * TILES_IN_ROW];
	int snake[TILES_IN_ROW * TILES_IN_ROW];
	int snakeHead, snakeSize;

	// everything relating to food
	uint score;
	int simpleFood;

	// snake movement
	Direction direction;		// the direction that we are attempting to move towards
	Direction currentDirection;	// the direction that we actually moved towards in the last time-step

	// other
	bool isHelpOn = false;
};


#endif // SNAKE_GAME_G_GRID_HPP
