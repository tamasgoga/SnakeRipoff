#ifndef FLAG_CORE_CORE_UTIL_HPP
#define FLAG_CORE_CORE_UTIL_HPP

//------------------------------------------------------------
// Utility functions and variables
//------------------------------------------------------------

#include "core_def.hpp"
#include <SDL2/SDL_rect.h>

namespace core {

	/** Find the center of an rectangle */
	inline float centerX(const SDL_Rect& rect) noexcept {return (rect.x + rect.x + rect.w) * 0.5f;}
	inline float centerY(const SDL_Rect& rect) noexcept {return (rect.y + rect.y + rect.h) * 0.5f;}

	/** Scale an SDL_Rect. Nothing happens if amount == 1 or amount <= 0. */
	SDL_Rect scale(SDL_Rect rect, float amount);


	/** Check if point is inside a rectangle */
	bool isInside(int x, int y, const SDL_Rect& rect);

	/** Ladies and gentleman, I give you the one and only "Fast Inverse Square Root" algorithm.
		Here for historical & inspirational purposes; not very useful when today's FPUs are so fast.

		Research (iterations):
			- 1: a bit faster than the FPU, but the result differs at the precision of 10^-4 and onwards
			- 2: quite slower than the FPU, but the result only differs by 1 at the precision of 10^-6
	*/
	float rsqrt(float x);

} // namespace core

#endif // FLAG_CORE_CORE_UTIL_HPP
