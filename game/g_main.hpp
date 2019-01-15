#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP


//--------------------------------------------------------------
// Version info
//--------------------------------------------------------------

#define GAME_VERSION_MAJOR	0
#define GAME_VERSION_MINOR	8
#define GAME_VERSION_BUILD	2

#define GAME_VERSION_NAME	"Beta"


//--------------------------------------------------------------
// Game code
//--------------------------------------------------------------

#include "../core/core_base.hpp"
#include "../core/core_ttf.hpp"

#include "g_grid.hpp"
#include "g_pulse.hpp"
#include "io_score.hpp"

#include <SDL2/SDL_events.h>


class Button;


namespace ui {

	extern core::AppDirPath* fontPath;

	extern core::Font* smallButtonFont;
	extern Button* quitButton;

} // namespace ui


class Game {
public:
	/** Constructor */
	Game();

	/** Returns false if the user wanted to terminate the app. */
	bool run(int speedUpMs);

private:
	// containers
	Grid grid;
	core::Texman texman;
	core::Font font;

	// general variables
	std::string scoreText;

	// text pulses
	Pulse scoreHighlight;
	SDL_Color scoreHighlightColor;

	Pulse frateHighlight;
	SDL_Color frateHighlightColor;

	// management
	core::SimpleTimer clock;
	State state;
	//------------- end of c-tor initializers -------------//
	int speedUpCounter; // accumulator (for checking ms passed)
	SDL_Event event;

	// textures
	core::texindex txBlackOverlay;
	core::texindex txWhiteOverlay;

	// fonts
	core::texindex ftScoreTitle;
	core::texindex ftScoreNumber;
	core::texindex ftScoreNumberHighlight;
	core::texindex ftFrateTitle;
	core::texindex ftFrateNumber;
	core::texindex ftFrateNumberHighlight;
	core::texindex ftPaused;
	core::texindex ftGameOver;

	int midFtPaused_w,		midFtPaused_h;
	int midFtGameOver_w,	midFtGameOver_h;
	int midFtScoreNumber_w,	midFtScoreNumber_h;

	// states
	void play(int speedUpMs);
	void pause();
	void endGame();

	// draw
	void drawGrid();
	void drawGameOver();

	// animations
	void flashScreenAndDelay();
	void collapseSnake();
};


bool showMenu();
bool showScores();


#endif // SNAKE_SNAKE_HPP
