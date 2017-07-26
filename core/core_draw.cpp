#include "core_draw.hpp"


namespace core {

	error drawPoint(int x, int y, int red, int green, int blue) noexcept {
		int err = SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		if (err == 0) {
			return SDL_RenderDrawPoint(renderer, x, y);
		} else {
			return err;
		}

	}


	error drawLine(int x1, int y1, int x2, int y2, int red, int green, int blue) noexcept {
		int err = SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		if (err == 0) {
			return SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		} else {
			return err;
		}
	}


	error drawRect(const SDL_Rect& rect, int red, int green, int blue) noexcept {
		int err = SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		if (err == 0) {
			return SDL_RenderDrawRect(core::renderer, &rect);
		} else {
			return err;
		}
	}


	error fillRect(const SDL_Rect& rect, int red, int green, int blue) noexcept {
		int err = SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		if (err == 0) {
			return SDL_RenderFillRect(renderer, &rect);
		} else {
			return err;
		}
	}


	error fillDisplay(int red, int green, int blue) noexcept {
		int err = SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
		if (err == 0) {
			return SDL_RenderFillRect(renderer, nullptr) == 0 ? true : false;
		} else {
			return err;
		}
	}


	error clearDisplay() noexcept {
		int err = SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
		if (err == 0) {
			return SDL_RenderClear(renderer);
		} else {
			return err;
		}
	}

} // namespace core
