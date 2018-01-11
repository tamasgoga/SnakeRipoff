#include "def.hpp"


namespace ui {

	SDL_Rect playArea;

	// not owners, just normal pointers, only use after core::main() was called
	core::texindex txSnake;
	core::texindex txSimpleFood;

	int speedLevel;

} // namespace ui
