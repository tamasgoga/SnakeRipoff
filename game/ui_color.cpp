#include "ui_color.hpp"


namespace ui {

	const SDL_Color WHITE	= {210, 210, 210, 255};
	const SDL_Color RED		= {191,  13,  79, 255};
	const SDL_Color BLUE	= { 19, 123, 158, 255};
	// yellow: 251, 167, 5 (title)


	SDL_Color tintColor(const SDL_Color& color, float factor) {
		bool lighten = true;

		if (factor < 0) {
			lighten = false;
			factor = -factor;
		}

		if (lighten) {
			return {
				uint8_t(color.r + (255 - color.r) * factor),
				uint8_t(color.g + (255 - color.g) * factor),
				uint8_t(color.b + (255 - color.b) * factor),
				color.a
			};
		}

		factor = 1.f + factor;
		return {
			uint8_t(color.r * factor),
			uint8_t(color.g * factor),
			uint8_t(color.b * factor),
			color.a
		};
	}


} // namespace ui
