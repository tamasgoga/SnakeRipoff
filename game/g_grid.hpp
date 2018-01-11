#ifndef SNAKE_GAME_G_GRID_HPP
#define SNAKE_GAME_G_GRID_HPP


//--------------------------------------------------------------
// In-game tile
//--------------------------------------------------------------

#include "def.hpp"
#include "g_tile.hpp"


constexpr int TILES_IN_ROW = 30;


namespace ui {

	extern core::Texman* gTexman;

} // namespace ui


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

	bool moveTile(int& tile);
	State advanceState();
	bool collapseSnakeTowardsItsMiddle();

	inline uint getScore() {return score;}

private:
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

	// buttons held
};


#endif // SNAKE_GAME_G_GRID_HPP
