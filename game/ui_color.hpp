#ifndef SNAKE_GAME_UI_COLOR_HPP
#define SNAKE_GAME_UI_COLOR_HPP


//--------------------------------------------------------------
// Color palette
//--------------------------------------------------------------

#include <SDL2/SDL_pixels.h>


namespace ui {

	constexpr SDL_Color WHITE = { 0xef, 0xef, 0xe3, 255 };
	constexpr SDL_Color GRAY  = { 0x21, 0x21, 0x21, 255 };
	constexpr SDL_Color BLACK = { 0x02, 0x02, 0x0a, 255 };

	constexpr SDL_Color RED    = { 0xbf, 0x0d, 0x5f, 255 };
	constexpr SDL_Color BLUE   = { 0x13, 0x7b, 0x9e, 255 };
	constexpr SDL_Color YELLOW = { 0xf7, 0xc9, 0x56, 255 };

	constexpr SDL_Color MARINE   = { 0x50, 0x32, 0xc9, 255 };
	constexpr SDL_Color PURPLE   = { 0x66, 0x55, 0xaa, 255 };
	constexpr SDL_Color BURGUNDY = { 0x7f, 0x0c, 0x32, 255 };
	constexpr SDL_Color CYAN     = { 0x51, 0xc1, 0xc6, 255 };
	constexpr SDL_Color GREEN    = { 0x37, 0x76, 0x77, 255 };


	/** Get a lighter or darker (factor < 0) shade of a color
	    Doesn't change the alpha */
	SDL_Color tintColor(const SDL_Color& color, float factor);

} // namespace ui


#endif // SNAKE_GAME_UI_COLOR_HPP
