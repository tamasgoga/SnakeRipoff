#ifndef SNAKE_GAME_UI_COLOR_HPP
#define SNAKE_GAME_UI_COLOR_HPP


//--------------------------------------------------------------
// Color palette
//--------------------------------------------------------------

#include <SDL2/SDL_pixels.h>


namespace ui {

	extern const SDL_Color WHITE;
	extern const SDL_Color RED;
	extern const SDL_Color BLUE;
	extern const SDL_Color GRAY;
	// yellow: 251, 167, 5 (title)


	/** Get a lighter or darker (factor < 0) shade of a color
	    Doesn't change the alpha */
	SDL_Color tintColor(const SDL_Color& color, float factor);

} // namespace ui


#endif // SNAKE_GAME_UI_COLOR_HPP
