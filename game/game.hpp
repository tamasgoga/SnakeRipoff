#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

//--------------------------------------------------------------
// Game code
//--------------------------------------------------------------


#include "def.hpp"
#include "entities.hpp"


inline int calculateTimeStep() {
	return 220 - ui::speedLevel*20;
}


/** Pulse with linear decay in milliseconds */
class Pulse {
public:
	using tstep_t = core::SimpleTimer::clock::rep;

	const uint64_t max;
	const uint64_t initDecay;
	const tstep_t timeStep;

	Pulse(uint64_t max, uint64_t initDecay, tstep_t timeStep);

	void reset();
	uint64_t get();

	inline bool pulsed() {
		return (value > 0) && (timer.elapsed_ms().count() / timeStep > 0);
	}

private:
	uint64_t decay;
	uint64_t value;
	core::SimpleTimer timer;
};


/* I don't advise instantiating more than 1 of this */
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
	core::texindex ftScoreNumberOverlay;
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
	void drawGrid(bool scoreChanged = false);
	void drawGameOver();

	// animations
	void flashScreenAndDelay();
	void collapseSnake();
};


bool menu();

#endif // SNAKE_SNAKE_HPP
