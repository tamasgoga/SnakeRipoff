#include "g_grid.hpp"

#include "../core/core_random.hpp"
#include "../core/core_error.hpp"

#include <SDL2/SDL_keyboard.h>


core::Random32 rng32(CORE_GET_RANDOM_SEED);


namespace ui {

	core::Texman* gTexman = nullptr;

	core::texindex txSnake;
	core::texindex txSimpleFood;
	core::texindex txHelpBackground;

	int speedLevel;
	SDL_Rect playArea;

} // namespace ui


Grid::Grid()
	: grid()
	, snake()
	, snakeHead(0)
	, snakeSize(1)
{
	// set additional variables
	int posx, posy, curr;
	posx = ui::playArea.x;
	posy = ui::playArea.y;

	// build grid
	for (int r = 0; r < TILES_IN_ROW; ++r) {
		for (int c = 0; c < TILES_IN_ROW; ++c) {
			curr = r * TILES_IN_ROW + c;

			grid[curr].x = posx;
			grid[curr].y = posy;

			posx += Tile::size;
		}

		posx = ui::playArea.x;
		posy += Tile::size;
	}

	int i = 12;
	const int bodyLength = i + INIT_SNAKE_SIZE;

	// head
	snake[0] = 15 * TILES_IN_ROW + i;
	grid[15 * TILES_IN_ROW + i].entity = Tile::SNAKE_HEAD;

	// body
	for (i = i + 1; i < bodyLength; ++i) {
		snake[snakeSize++] = 15 * TILES_IN_ROW + i;
		grid[15 * TILES_IN_ROW + i].entity = Tile::SNAKE_BODY;
	}

	// movement
	direction = Direction::LEFT;
	currentDirection = Direction::LEFT;

	generateFood();
	score = 0;
}


// returns State::WON if no more food can be generated
State Grid::generateFood() {
	// won
	if (snakeSize == MAX)
		return State::WON;

	int freeTileCount = MAX - snakeSize;

	if (freeTileCount > TILES_IN_ROW) {
		// generate a random number
		do {
			simpleFood = core::randomInt(rng32, 0, MAX - 1);
		} while (grid[simpleFood].entity != Tile::NONE);

	} else {
		// the snake is very large, pick one of the empty spaces
		int emptyTiles[TILES_IN_ROW];
		int emptyTilesCount = 0;

		for (int i = 0; i < MAX; ++i) {
			if (grid[i].entity == Tile::NONE) {
				emptyTiles[emptyTilesCount++] = i;

				if (emptyTilesCount == TILES_IN_ROW)
					break;
			}
		}

		simpleFood = emptyTiles[core::randomInt(rng32, 0, emptyTilesCount - 1)];
	}

	grid[simpleFood].entity = Tile::SIMPLE_FOOD;

	return State::PLAYING;
}


void Grid::draw() const {
	for (auto tile: grid) {
		switch (tile.entity) {
		case Tile::SNAKE_HEAD:
			ui::gTexman->draw(ui::txSnake, tile);
			break;

		case Tile::SNAKE_BODY:
			ui::gTexman->draw(ui::txSnake, tile.x + 1, tile.y + 1);
			break;

		case Tile::SIMPLE_FOOD:
			ui::gTexman->draw(ui::txSimpleFood, tile.x + 2, tile.y + 2);
			break;

		default:
			if (isHelpOn) {
				ui::gTexman->draw(ui::txHelpBackground, tile.x + 1, tile.y + 1);
			}
			break;
		}
	}
}


void Grid::handleKeyboard() {
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);

	// vertical movement
	if (currentDirection == Direction::UP || currentDirection == Direction::DOWN) {
		if (keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT]) {
			direction = Direction::LEFT;
		} else if (keystates[SDL_SCANCODE_RIGHT] && !keystates[SDL_SCANCODE_LEFT]) {
			direction = Direction::RIGHT;
		}
	}

	// horizontal movement
	else if (currentDirection == Direction::LEFT || currentDirection == Direction::RIGHT) {
		if (keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN]) {
			direction = Direction::UP;
		} else if (keystates[SDL_SCANCODE_DOWN] && !keystates[SDL_SCANCODE_UP]) {
			direction = Direction::DOWN;
		}
	}

	// help toggle
	if (keystates[SDL_SCANCODE_F1]) {
		isHelpOn = !isHelpOn;
	}
}


// returns a game state
State Grid::advanceState() {
	if (snakeSize == 0)
		throw core::Failure("How the f*ck wasn't the snake initialized?! I had one job!!");

	// next & previous positions (obviously the head at the beginning)
	int prev = snake[0];

	// move head
	switch (direction) {
	case Direction::UP:
		if (prev < TILES_IN_ROW)
			snake[0] = (TILES_IN_ROW - 1) * TILES_IN_ROW + prev;
		else
			snake[0] = prev - TILES_IN_ROW;
		break;

	case Direction::DOWN:
		if (prev >= (TILES_IN_ROW - 1) * TILES_IN_ROW)
			snake[0] = prev % TILES_IN_ROW;
		else
			snake[0] = prev + TILES_IN_ROW;
		break;

	case Direction::LEFT:
		if (prev % TILES_IN_ROW == 0)
			snake[0] = prev + (TILES_IN_ROW - 1);
		else
			snake[0] -= 1;
		break;

	case Direction::RIGHT:
		if (prev % TILES_IN_ROW == TILES_IN_ROW - 1)
			snake[0] = prev - (TILES_IN_ROW - 1);
		else
			snake[0] += 1;
		break;

	default: break;
	}

	// check if food, move head
	bool newFoodNeeded = false;
	if (grid[snake[0]].entity == Tile::SIMPLE_FOOD) {
		incScore();
		newFoodNeeded = true;
	} else if (grid[snake[0]].entity != Tile::NONE) {
		snake[0] = prev;
		return State::OVER;
	}

	grid[snake[0]].entity = Tile::SNAKE_HEAD;

	// move rest
	int temp;
	for (int i = 1; i < snakeSize; ++i) {
		temp = snake[i];
		snake[i] = prev;
		prev = temp;

		grid[snake[i]].entity = Tile::SNAKE_BODY;
	}

	// set the direction
	currentDirection = direction;

	// generate food, body parts
	if (newFoodNeeded) {
		snake[snakeSize++] = prev;
		return generateFood();
	} else {
		grid[prev].entity = Tile::NONE;
	}

	return State::PLAYING;
}


// used when killing the snake; returns false if the snake collapsed already
bool Grid::collapseSnakeTowardsItsMiddle() {
	if (snakeHead > snakeSize)
		return false;

	// collapse
	grid[snake[snakeHead]].entity = Tile::NONE;
	grid[snake[snakeSize--]].entity = Tile::NONE;

	// size matters :p
	grid[snake[++snakeHead]].entity = Tile::SNAKE_HEAD;

	return true;
}
