#include "ui_button.hpp"

#include "../core/core_draw.hpp"
#include "../core/core_util.hpp"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_events.h>


Button::Button(core::Font& font, std::string text, const SDL_Rect& box)
	: box(box)
	, state(NORMAL)
	, text(text)
	, font(font)
{
	ftText = font.loadText(text, normal);
	midFtText_w = (2*box.x + box.w) / 2 - font.getWidth(ftText) / 2;
	midFtText_h = (2*box.y + box.h) / 2 - font.getHeight(ftText) / 2;
}


Button::Button(const Button& other)
	: box(other.box)
	, state(other.state)
	, text(other.text)
	, font(other.font)
{
	ftText = ftText = font.loadText(text, normal);
	midFtText_w = other.midFtText_w;
	midFtText_h = other.midFtText_h;
}


Button::Button(Button&& other)
	: box(other.box)
	, state(other.state)
	, text(other.text)
	, font(other.font)
	, ftText(other.ftText)
	, midFtText_w(other.midFtText_w)
	, midFtText_h(other.midFtText_h)
{
	// empty text signals move
	other.text.clear();
}


Button::~Button() {
	// empty text signals move
	if (!text.empty())
		font.remove(ftText);
}


void Button::draw() const {
	switch(state) {
	case NORMAL:
		core::setDrawColor(normal.r, normal.g, normal.b);
		break;
	case CLICKED:
	case HOVER:
		core::setDrawColor(hover.r, hover.g, hover.b);
		break;
	case SELECTED:
		core::setDrawColor(select.r, select.g, select.b);
		break;
	}

	core::drawRect(box);
	font.draw(ftText, midFtText_w, midFtText_h);
}


Button::ButtonState Button::handleMouse(SDL_Event& event) noexcept {
	// mouse
	int mouseX, mouseY;
	Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	if (core::isInside(mouseX, mouseY, box)) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				state = SELECTED;
			else
				state = HOVER;
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				state = CLICKED;
			else
				state = HOVER;
			break;

		case SDL_MOUSEMOTION:
			if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
				state = SELECTED;
			else
				state = HOVER;
			break;

		default:
			state = HOVER;
			break;
		}
	} else {
		state = NORMAL;
	}

	return state;
}


SDL_Color Button::normal = ui::WHITE;
SDL_Color Button::hover = ui::BLUE;
SDL_Color Button::select = ui::RED;
