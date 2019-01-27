#include "g_grid.hpp"

#include "../core/core_texture.hpp"
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

	int tileSize;

} // namespace ui


Grid::Grid()
	: grid()
	, snake()
	, snakeHead(0)
	, snakeSize(0)
{
	static constexpr int START_ROW = 15;
	static constexpr int START_COL = 12;

	for (int i = 0; i < INIT_SNAKE_SIZE; ++i) {
		int pos = START_ROW * TILES_IN_ROW + i + START_COL;
		snake[snakeSize++] = pos;
		grid[pos].makeSnake(i + 1);
	}

	direction = Direction::LEFT;
	currentDirection = Direction::LEFT;

	generateFood();
	score = 0;
}


// returns State::WON if no more food can be generated
State Grid::generateFood() {
	if (snakeSize == MAX) {
		return State::WON;
	}

	int freeTileCount = MAX - snakeSize;

	if (freeTileCount > TILES_IN_ROW) {
		// generate a random number
		do {
			simpleFood = core::randomInt(rng32, 0, MAX - 1);
		} while (!grid[simpleFood].isEmpty());
	} else {
		// the snake is very large, pick one of the empty spaces
		int emptyTiles[TILES_IN_ROW];
		int emptyTilesCount = 0;

		for (int i = 0; i < MAX; ++i) {
			if (grid[i].isEmpty()) {
				emptyTiles[emptyTilesCount++] = i;

				if (emptyTilesCount == TILES_IN_ROW) {
					break;
				}
			}
		}
		simpleFood = emptyTiles[core::randomInt(rng32, 0, emptyTilesCount - 1)];
	}

	grid[simpleFood].makeSimpleFood();

	return State::PLAYING;
}


void Grid::draw() const {
	int posX = ui::playArea.x;
	int posY = ui::playArea.y;
	SDL_Rect fullTile = { posX, posY, ui::tileSize, ui::tileSize };

	int curr;

	for (int r = 0; r < TILES_IN_ROW; ++r) {
		for (int c = 0; c < TILES_IN_ROW; ++c) {
			curr = r * TILES_IN_ROW + c;

			if (grid[curr].isSnakeHead()) {
				fullTile.x = posX;
				fullTile.y = posY;
				ui::gTexman->draw(ui::txSnake, fullTile);
			} else if (grid[curr].isSnakeBody()) {
				ui::gTexman->draw(ui::txSnake, posX + 1, posY + 1);
			} else if (grid[curr].isSimpleFood()) {
				ui::gTexman->draw(ui::txSimpleFood, posX + 2, posY + 2);
			} else if (isHelpOn) {
				ui::gTexman->draw(ui::txHelpBackground, posX + 1, posY + 1);
			}

			posX += ui::tileSize;
		}

		posX = ui::playArea.x;
		posY += ui::tileSize;
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
	if (grid[snake[0]].isSimpleFood()) {
		incScore();
		newFoodNeeded = true;
	} else if (!grid[snake[0]].isEmpty()) {
		snake[0] = prev;
		return State::OVER;
	}

	grid[snake[0]].makeSnakeHead();

	// move rest
	int temp;
	for (int i = 1; i < snakeSize; ++i) {
		temp = snake[i];
		snake[i] = prev;
		prev = temp;

		grid[snake[i]].makeSnake(i + 1);
	}

	// set the direction
	currentDirection = direction;

	// generate food, body parts
	if (newFoodNeeded) {
		snake[snakeSize++] = prev;
		return generateFood();
	} else {
		grid[prev].makeEmpty();
	}

	return State::PLAYING;
}


// used when killing the snake; returns false if the snake collapsed already
bool Grid::collapseSnakeTowardsItsMiddle() {
	if (snakeHead > snakeSize)
		return false;

	// collapse
	grid[snake[snakeHead]].makeEmpty();
	grid[snake[snakeSize--]].makeEmpty();

	// size matters :p
	grid[snake[++snakeHead]].makeSnakeHead();

	return true;
}


uint Grid::getScore() noexcept {
	return score;
}


void Grid::incScore() noexcept {
	score += ui::speedLevel;
}
