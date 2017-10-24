#ifndef SNAKE_ENTITY_HPP
#define SNAKE_ENTITY_HPP

//--------------------------------------------------------------
// Game & menu entities
//--------------------------------------------------------------


#include "def.hpp"


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


class Button {
public:
	enum ButtonState {
		NORMAL,
		HOVER,
		SELECTED,   // left mouse button DOWN
		CLICKED     // left mouse button UP
	};

	Button(core::Font& font, std::string text, const SDL_Rect& box);
	Button(const Button& other);
	Button(Button&& other);

	virtual ~Button();

	void draw() const;
	ButtonState handleMouse(SDL_Event& event) noexcept;
	inline bool isState(ButtonState bs) const noexcept {return state == bs;}

	friend class Slider;

private:
	SDL_Rect box;
	ButtonState state;

	std::string text;
	core::Font& font;
	core::texindex ftText;
	int midFtText_w, midFtText_h;

	static SDL_Color normal, hover, select;
};


class Slider {
public:
	static const int MIN = 0;
	static const int MAX = 10;

	Slider(core::Font& font, int& sliderVariable, int x, int y);

	inline void inc() {if (level < MAX) ++level;}
	inline void dec() {if (level > MIN) --level;}

	void handleMouse(SDL_Event& event);
	void draw() const;

private:
	Button decButton, incButton;
	int& level;
	bool mousedOver;
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

#endif // SNAKE_ENTITY_HPP
