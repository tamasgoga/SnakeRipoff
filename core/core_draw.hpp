#ifndef FLAG_CORE_CORE_DRAW_HPP
#define FLAG_CORE_CORE_DRAW_HPP

//------------------------------------------------------------
// Drawing primitives onto, filling, or updating the screen
//------------------------------------------------------------


#include "core_def.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>

#include <vector>

struct SDL_Surface;


namespace core {

	/** The renderer of the application. */
	extern SDL_Renderer* renderer;


	/** Set the drawing color of the renderer. */
	inline error setDrawColor(int red, int green, int blue) noexcept {
		return SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	}


	/** Draw a point with the current drawing color. */
	inline error drawPoint(int x, int y) noexcept {
		return SDL_RenderDrawPoint(renderer, x, y);
	}


	/** Draw a point with the specified color. */
	error drawPoint(int x, int y, int red, int green, int blue) noexcept;


	/** Draw a line with the current drawing color. */
	inline error drawLine(int x1, int y1, int x2, int y2) noexcept {
		return SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}


	/** Draw a line with the specified color. */
	error drawLine(int x1, int y1, int x2, int y2, int red, int green, int blue) noexcept;


	/** Draw a rectangle outline with the current drawing color. */
	inline error drawRect(const SDL_Rect& rect) noexcept {
		return SDL_RenderDrawRect(renderer, &rect);
	}


	/** Draw a rectangle outline with the specified color. */
	error drawRect(const SDL_Rect& rect, int red, int green, int blue) noexcept;


	/** Draw a filled rectangle with the current drawing color. */
	inline error fillRect(const SDL_Rect& rect) noexcept {
		return SDL_RenderFillRect(renderer, &rect);
	}


	/** Draw a filled rectangle with the specified color. */
	error fillRect(const SDL_Rect& rect, int red, int green, int blue) noexcept;


	/** Fill the window with the specified color. */
	error fillDisplay(int red, int green, int blue) noexcept;


	/** Clear the screen to black. */
	error clearDisplay() noexcept;


	/** Update the window with the contents of the frame-buffer. */
	inline void updateDisplay() noexcept {
		SDL_RenderPresent(renderer);
	}

} // namespace core

#endif // FLAG_CORE_CORE_DRAW_HPP
