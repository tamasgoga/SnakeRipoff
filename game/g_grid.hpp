#ifndef SNAKE_GAME_G_GRID_HPP
#define SNAKE_GAME_G_GRID_HPP


//--------------------------------------------------------------
// In-game tile
//--------------------------------------------------------------

#include "../core/core_def.hpp"

#include "g_state.hpp"
#include "g_tile.hpp"


enum class Direction {
	NONE	= 0x0,	// 0000

	UP		= 0x1,	// 0001
	DOWN	= 0x2,	// 0010
	LEFT	= 0x4,	// 0100
	RIGHT	= 0x8	// 1000
};


class Grid {
public:
	static constexpr int TILES_IN_ROW       = 30;
	static constexpr int MAX				= TILES_IN_ROW * TILES_IN_ROW;
	static constexpr int INIT_SNAKE_SIZE	= 4;

	// interface
	Grid();

	// (!) lack of constructors

	State generateFood();

	void draw() const;
	void handleKeyboard();

	State advanceState();
	bool collapseSnakeTowardsItsMiddle();

	// (!) the grid should not know the score?
	uint getScore() noexcept;

private:
	// (!) the grid should not know the score?
	void incScore() noexcept;

	// (!) dude, why so many arrays?! why not std::array?
	// tiles & snake
	Tile grid[TILES_IN_ROW * TILES_IN_ROW];
	// (!) get rid of second array! NOW!
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
