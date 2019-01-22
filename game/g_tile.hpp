#ifndef SNAKE_GAME_G_TILE_HPP
#define SNAKE_GAME_G_TILE_HPP


//--------------------------------------------------------------
// In-game tile
//--------------------------------------------------------------

#include <cstdint>


class Tile {
public:
	Tile() noexcept
		: value(0)
	{}

	Tile(const Tile&) = default;
	Tile& operator=(const Tile&) = default;

	Tile(Tile&&) = default;
	Tile& operator=(Tile&&) = default;


	// Empty

	void makeEmpty() noexcept {
		value = uint32_t(0);
	}

	bool isEmpty() const noexcept {
		return value == uint32_t(0);
	}


	// Simple food

	void makeSimpleFood() noexcept {
		value |= uint32_t(1);
	}

	void clearSimpleFood() noexcept {
		value &= ~uint32_t(1);
	}
	
	bool isSimpleFood() const noexcept {
		return value & uint32_t(1);
	}


	// Snake (generic)

	void makeSnake(uint16_t snakeIndex) noexcept {
		value = (uint32_t(snakeIndex) << 16) | getFlags();
	}

	// Returns the index in the snake's body
	uint32_t getSnakeIndex() const noexcept {
		return value >> 16;
	}

	bool isSnake() const noexcept {
		return getSnakeIndex() > uint32_t(0);
	}


	// Snake HEAD

	void makeSnakeHead() noexcept {
		makeSnake(1);
	}

	bool isSnakeHead() const noexcept {
		return getSnakeIndex() == uint32_t(1);
	}


	// Snake BODY

	bool isSnakeBody() const noexcept {
		return getSnakeIndex() > uint32_t(1);
	}

private:
	/**
	 * Lower-half: flags (like simple-food)
	 * Upper-half: snake-body-part index
	 */
	uint32_t value;

	uint32_t getFlags() const noexcept {
		return value & uint32_t(0xffff);
	}
};


#endif // SNAKE_GAME_G_TILE_HPP
