#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include "def.hpp"
#include "entities.hpp"

inline int calculateTimeStep() {
	return 220 - ui::speedLevel*20;
}

class Game {
public:
	/** Constructor */
	Game();

	/** Returns false if the user wanted to terminate the app. */
	bool run();

private:
	// containers
	Grid grid;
	core::Texman texman;
	core::Font font;

	// general variables
	std::string scoreText;

	// management
	core::SimpleTimer clock;
	State state;
	int timeStep;
	SDL_Event event;

	// textures
	core::texindex txBlackOverlay;
	core::texindex txWhiteOverlay;

	// fonts
	core::texindex ftScoreTitle;
	core::texindex ftScoreNumber;
	core::texindex ftFrateTitle;
	core::texindex ftFrateNumber;
	core::texindex ftPaused;
	core::texindex ftGameOver;

	int midFtPaused_w,		midFtPaused_h;
	int midFtGameOver_w,	midFtGameOver_h;
	int midFtScoreNumber_w,	midFtScoreNumber_h;

	// states
	void play();
	void pause();
	void endGame();

	// draw
	void drawGrid();
	void drawGameOver();

	// animations
	void flashScreenAndDelay();
	void collapseSnake();
};

bool menu();

#endif // SNAKE_SNAKE_HPP
