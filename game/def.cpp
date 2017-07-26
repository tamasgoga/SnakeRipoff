#include "def.hpp"

core::Random32 rng32(CORE_GET_RANDOM_SEED);

namespace ui {

	SDL_Rect playArea;

	const SDL_Color WHITE	= {210, 210, 210, 255};
	const SDL_Color RED		= {191,  13,  79, 255};
	const SDL_Color BLUE	= { 19, 123, 158, 255};
	// yellow: 251, 167, 5 (title)

	// not owners, just normal pointers, only use after core::main() was called

	core::AppDirPath* fontPath = nullptr;

	core::Font* smallButtonFont = nullptr;
	Button* quitButton = nullptr;

	core::Texman* gTexman = nullptr;
	core::texindex txTitle;
	core::texindex txSnake;
	core::texindex txSimpleFood;

	int speedLevel;

} // namespace ui
