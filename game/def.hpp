#ifndef SNAKE_DEF_HPP
#define SNAKE_DEF_HPP


//--------------------------------------------------------------
// Includes & definitions
//--------------------------------------------------------------

#include "../core/core.hpp"

#include <SDL2/SDL_rect.h>


//--------------------------------------------------------------
// Enumerations
//--------------------------------------------------------------

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


//--------------------------------------------------------------
// User interface variables
//--------------------------------------------------------------

class Button;

namespace ui {

	extern SDL_Rect playArea;

	extern core::texindex txSnake;
	extern core::texindex txSimpleFood;

	extern int speedLevel;

} // namespace ui

#endif // SNAKE_DEF_HPP
