#ifndef SNAKE_GAME_UI_COLOR_HPP
#define SNAKE_GAME_UI_COLOR_HPP


//--------------------------------------------------------------
// Color palette
//--------------------------------------------------------------

#include <SDL2/SDL_pixels.h>


namespace ui {

	constexpr SDL_Color WHITE  = { 210, 210, 210, 255 };
	constexpr SDL_Color RED    = { 191,  13,  79, 255 };
	constexpr SDL_Color BLUE   = {  19, 123, 158, 255 };
	constexpr SDL_Color GRAY   = {  33,  33,  33, 255 };
	constexpr SDL_Color YELLOW = { 251, 167,   5, 255 };


	/** Get a lighter or darker (factor < 0) shade of a color
	    Doesn't change the alpha */
	SDL_Color tintColor(const SDL_Color& color, float factor);

} // namespace ui


#endif // SNAKE_GAME_UI_COLOR_HPP
