#ifndef SNAKE_GAME_UI_BUTTON_HPP
#define SNAKE_GAME_UI_BUTTON_HPP


//--------------------------------------------------------------
// Button
//--------------------------------------------------------------

#include "../core/core_ttf.hpp"
#include <SDL2/SDL_rect.h>
#include <string>

union SDL_Event;


class Button {
public:
	enum ButtonState {
		NORMAL,
		HOVER,
		SELECTED,   // left mouse button DOWN
		CLICKED     // left mouse button UP
	};

	Button(core::Font& font, std::string text, const SDL_Rect& box);
	Button(const Button& other);
	Button(Button&& other);

	virtual ~Button();

	void draw() const;
	ButtonState handleMouse(SDL_Event& event) noexcept;

	inline bool isState(ButtonState bs) const noexcept {
		return state == bs;
	}

	inline void setState(ButtonState bs) noexcept {
		state = bs;
	}

	inline const SDL_Rect& getBoundingBox() const noexcept {
		return box;
	}

private:
	SDL_Rect box;
	ButtonState state;

	std::string text;
	core::Font& font;
	core::texindex ftText;
	int midFtText_w, midFtText_h;

	static SDL_Color normal, hover, select;
};


#endif // SNAKE_GAME_UI_BUTTON_HPP
