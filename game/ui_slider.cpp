#include "ui_slider.hpp"

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_events.h>


const int Slider::MIN = 0;
const int Slider::MAX = 10;


namespace ui {

	extern core::Texman* gTexman;

	extern core::texindex txSnake;
	extern core::texindex txSimpleFood;

	extern int tileSize;

} // namespace ui


Slider::Slider(core::Font& font, int& sliderVariable, int x, int y)
	: decButton(font, "-", {x, y, ui::tileSize, ui::tileSize})
	, incButton(font, "+", {x + ui::tileSize*13, y, ui::tileSize, ui::tileSize})
	, level(sliderVariable)
	, mousedOver(false)
{
	if (level < MIN) {
		level = MIN;
	} else if (level > MAX) {
		level = MAX;
	}

	const auto& incBox = incButton.getBoundingBox();
	const auto& decBox = decButton.getBoundingBox();
	box = {
		decBox.x,
		decBox.y,
		incBox.x + incBox.w - decBox.x,
		incBox.h
	};
}


void Slider::handleMouse(SDL_Event& event) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	// check if mouse is inside the slider
	if (mx >= box.x && mx <= box.x + box.w && my >= box.y && my <= box.y + box.h) {
		mousedOver = true;

		// mouse-wheel event
		if (event.type == SDL_MOUSEWHEEL) {
			level += event.wheel.y;

			if (level < MIN)
				level = MIN;
			else if (level > MAX)
				level = MAX;
		}
	} else {
		mousedOver = false;
	}

	if (decButton.handleMouse(event) == Button::CLICKED) {
		dec();
	} else if (incButton.handleMouse(event) == Button::CLICKED) {
		inc();
	}
}


void Slider::draw() const {
	decButton.draw();
	int posx = box.x + 2*ui::tileSize;

	for (int i = 0; i < level; ++i) {
		if (mousedOver)
			ui::gTexman->draw(ui::txSimpleFood, {posx + 1, box.y + 1, ui::tileSize - 2, ui::tileSize - 2});
		else
			ui::gTexman->draw(ui::txSnake, posx + 1, box.y + 1);
		posx += ui::tileSize;
	}

	incButton.draw();
}
