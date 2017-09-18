#ifndef SNAKE_DEF_HPP
#define SNAKE_DEF_HPP

//--------------------------------------------------------------
// Includes & definitions
//--------------------------------------------------------------


#include "../core/core.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>


//--------------------------------------------------------------
// Management
//--------------------------------------------------------------

#define GAME_VERSION_MAJOR	0
#define GAME_VERSION_MINOR	7
#define GAME_VERSION_BUILD	8

#define GAME_VERSION_NAME	"Classy Folded Code"

constexpr int TILES_IN_ROW = 30;

extern core::Random32 rng32;


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

	extern const SDL_Color WHITE;
	extern const SDL_Color RED;
	extern const SDL_Color BLUE;
	// yellow: 251, 167, 5 (title)

	// not owners, just normal pointers, only use after core::main() was called
	extern core::AppDirPath* fontPath;

	extern core::Font* smallButtonFont;
	extern Button* quitButton;

	extern core::Texman* gTexman;
	extern core::texindex txTitle;
	extern core::texindex txSnake;
	extern core::texindex txSimpleFood;

	extern int speedLevel;

} // namespace ui

#endif // SNAKE_DEF_HPP
