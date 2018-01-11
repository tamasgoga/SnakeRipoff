#include "ui_slider.hpp"
#include "g_tile.hpp"
#include "def.hpp"

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_events.h>


const int Slider::MIN = 0;
const int Slider::MAX = 10;


namespace ui {

	extern core::Texman* gTexman;

} // namespace ui


Slider::Slider(core::Font& font, int& sliderVariable, int x, int y)
	: decButton(font, "-", {x, y, Tile::size, Tile::size})
	, incButton(font, "+", {x + Tile::size*13, y, Tile::size, Tile::size})
	, level(sliderVariable)
	, mousedOver(false)
{
	if (level < MIN) {
		level = MIN;
	} else if (level > MAX) {
		level = MAX;
	}
}


void Slider::handleMouse(SDL_Event& event) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	// check if mouse is inside
	if (mx >= decButton.box.x && mx <= incButton.box.x + incButton.box.w &&
	    my >= decButton.box.y && my <= decButton.box.y + decButton.box.h)
	{
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
	int posx = decButton.box.x + 2*Tile::size;

	for (int i = 0; i < level; ++i) {
		if (mousedOver)
			ui::gTexman->draw(ui::txSimpleFood, {posx + 1, decButton.box.y + 1, Tile::size - 2, Tile::size - 2});
		else
			ui::gTexman->draw(ui::txSnake, posx + 1, decButton.box.y + 1);
		posx += Tile::size;
	}

	incButton.draw();
}
