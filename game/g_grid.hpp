#ifndef SNAKE_GAME_G_GRID_HPP
#define SNAKE_GAME_G_GRID_HPP


//--------------------------------------------------------------
// In-game tile
//--------------------------------------------------------------

#include "../core/core_def.hpp"

#include "g_state.hpp"
#include "g_tile.hpp"

#include <array>


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

	Grid();

	Grid(const Grid&) = default;
	Grid& operator=(const Grid&) = default;

	Grid(Grid&&) = default;
	Grid& operator=(Grid&&) = default;

	State generateFood() noexcept;

	void draw() const noexcept;
	void handleKeyboard() noexcept;

	State advanceState();
	bool collapseSnakeTowardsItsMiddle() noexcept;

	uint getScore() const noexcept;

private:
	void incScore() noexcept;

	// tiles & snake
	std::array<Tile, MAX> grid;
	std::array<int, MAX> snake;
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
