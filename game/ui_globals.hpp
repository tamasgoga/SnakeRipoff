#ifndef SNAKE_GAME_UI_GLOBALS_HPP
#define SNAKE_GAME_UI_GLOBALS_HPP


//--------------------------------------------------------------
// Globals used by everything
//--------------------------------------------------------------

#include <SDL2/SDL_rect.h>
#include "../core/core_texture.hpp"

class Button;

namespace core {

    struct AppDirPath;
    class Font;

}

/**
 * I'll be working towards getting rid of as many of these as possible.
 * Maybe the some fancy "dependency-injection" would solve most of our issues.
 */

namespace ui {

    //--------------------------------------------------------------
    // g_grid.cpp
    //--------------------------------------------------------------

	extern core::Texman* gTexman;

	extern core::texindex txSnake;
	extern core::texindex txSimpleFood;
	extern core::texindex txHelpBackground;

	extern int speedLevel;
	extern SDL_Rect playArea;

	extern int tileSize;


    //--------------------------------------------------------------
    // g_main.cpp
    //--------------------------------------------------------------

    extern core::AppDirPath* fontPath;

	extern core::Font* smallButtonFont;
	extern Button* quitButton;

}


#endif // SNAKE_GAME_UI_GLOBALS_HPP
