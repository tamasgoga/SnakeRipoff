#ifndef SNAKE_GAME_UI_SLIDER_HPP
#define SNAKE_GAME_UI_SLIDER_HPP


//--------------------------------------------------------------
// Slider
//--------------------------------------------------------------

#include "ui_button.hpp"


class Slider {
public:
	static const int MIN;
	static const int MAX;

	Slider(core::Font& font, int& sliderVariable, int x, int y);

	inline void inc() {if (level < MAX) ++level;}
	inline void dec() {if (level > MIN) --level;}

	void handleMouse(SDL_Event& event);
	void draw() const;

private:
	Button decButton, incButton;
	int& level;
	bool mousedOver;
};


#endif // SNAKE_GAME_UI_SLIDER_HPP
