#include "game.hpp"

//--------------------------------------------------------------
// Class: Game
//--------------------------------------------------------------


Game::Game()
	: grid(), texman(), font(ui::fontPath->path.c_str(), 20), scoreText("00000"), clock(true), state(State::PLAYING)
{
	using namespace core;

	txBlackOverlay = texman.create(getWindowWidth(), getWindowHeight(), 0,0,0);
	texman.setAlphaMod(txBlackOverlay, 170);
	txWhiteOverlay = texman.create(getWindowWidth(), getWindowHeight(), 255, 255, 255);

	ftScoreTitle = font.loadText("SCORE", ui::WHITE);
	ftScoreNumber = font.loadText(scoreText, ui::WHITE);
	ftFrateTitle = font.loadText("TSTEP", ui::WHITE);
	ftFrateNumber = font.loadText("?", ui::WHITE);
	ftPaused = font.loadText("PAUSED", ui::WHITE);
	ftGameOver = font.loadText("GAME OVER", ui::WHITE);

	int midWindow_w = getWindowWidth() / 2;
	int midWindow_h = getWindowHeight() / 2;

	midFtPaused_w = midWindow_w - font.getWidth(ftPaused) / 2;
	midFtPaused_h = midWindow_h - font.getHeight(ftPaused) / 2;
	midFtGameOver_w = midWindow_w - font.getWidth(ftGameOver) / 2;
	midFtGameOver_h = midWindow_h - font.getHeight(ftGameOver) / 2 - 10;
	midFtScoreNumber_w = midWindow_w - font.getWidth(ftScoreNumber) / 2;
	midFtScoreNumber_h = midWindow_h - font.getHeight(ftScoreNumber) / 2 + 10;

	timeStep = calculateTimeStep();

	// initial draw
	clearDisplay();
	drawGrid();
	ui::quitButton->draw();
	updateDisplay();
}


bool Game::run() {
	using namespace core;

	while(true) {
		switch (state) {
		case State::PLAYING:
			play();
			break;

		case State::PAUSED:
			pause();
			break;

		case State::OVER:
			flashScreenAndDelay();
			collapseSnake();
			endGame();
			break;

		case State::WON:
			endGame();	// do a proper thing here
			break;

		case State::MENU:
			return true;

		case State::QUIT:
			return false;
		}
	}
}


void Game::play() {
	using namespace core;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
				state = State::PAUSED;
				return;
			}
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				state = State::PAUSED;
				return;
			} // event.key.keysym.sym

			grid.handleKeyboard();

			break;

		case SDL_QUIT:
			state = State::QUIT;
			return;

		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			ui::quitButton->handleMouse(event);
			break;

		} // switch event.type
	}

	/// STATE MANAGEMENT & TIMERS ///

	// quit
	if (ui::quitButton->isState(Button::CLICKED)) {
		state = State::MENU;
		return;
	}

	// skip the rest, unless reached time-step (or first)
	if (clock.elapsed_ms().count() < timeStep)
		return;
	clock.reset();

	/// GAME LOGIC ///

	state = grid.advanceState();

	// the score
	if (grid.getScore() < 10) {
		scoreText = "0000" + std::to_string(grid.getScore());
	} else if (grid.getScore() < 100) {
		scoreText = "000" + std::to_string(grid.getScore());
	} else if (grid.getScore() < 1000) {
		scoreText = "00" + std::to_string(grid.getScore());
	} else if (grid.getScore() < 10000) {
		scoreText = "0" + std::to_string(grid.getScore());
	} else {
		scoreText = std::to_string(grid.getScore());
	}

	font.changeText(ftScoreNumber, scoreText, ui::WHITE);
	font.changeText(ftFrateNumber, std::to_string(timeStep), ui::WHITE);

	/// RENDERING ///

	clearDisplay();
	drawGrid();
	ui::quitButton->draw();
	updateDisplay();
}


void Game::pause() {
	using namespace core;

	clearDisplay();
	drawGrid();

	texman.draw(txBlackOverlay, 0, 0);
	font.draw(ftPaused, midFtPaused_w, midFtPaused_h);

	ui::quitButton->draw();
	updateDisplay();

	while (SDL_WaitEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				state = State::QUIT;
				return;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_RETURN:
				case SDLK_ESCAPE:
					state = State::PLAYING;
					return;
				}

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				ui::quitButton->handleMouse(event);
				break;
		}

		if (ui::quitButton->isState(Button::CLICKED)) {
			state = State::MENU;
			return;
		}

		clearDisplay();

		drawGrid();

		texman.draw(txBlackOverlay, 0, 0);
		font.draw(ftPaused, midFtPaused_w, midFtPaused_h);

		ui::quitButton->draw();
		updateDisplay();
	}
}


void Game::endGame() {
	using namespace core;

	clearDisplay();
	drawGameOver();
	ui::quitButton->draw();
	updateDisplay();

	while (SDL_WaitEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				state = State::QUIT;
				return;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_RETURN:
				case SDLK_ESCAPE:
					state = State::MENU;
					return;
				}

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				ui::quitButton->handleMouse(event);
				break;
		}

		if (ui::quitButton->isState(Button::CLICKED)) {
			state = State::MENU;
			return;
		}

		clearDisplay();
		drawGameOver();
		ui::quitButton->draw();
		updateDisplay();
	}
}


void Game::drawGrid() {
	grid.draw();
	core::drawRect(ui::playArea, ui::WHITE.r, ui::WHITE.g, ui::WHITE.g);

	font.draw(ftScoreTitle, 10, 10);
	font.draw(ftScoreNumber, 10, 30);
	font.draw(ftFrateTitle, 10, 80);
	font.draw(ftFrateNumber, 10, 100);
}


void Game::drawGameOver() {
	texman.draw(txBlackOverlay, 0, 0);
	font.draw(ftGameOver, midFtGameOver_w, midFtGameOver_h);
	font.draw(ftScoreNumber, midFtScoreNumber_w, midFtScoreNumber_h);
}


void Game::flashScreenAndDelay() {
	using namespace core;

	// flash of white (hue-hue, flash of light)
	texman.draw(txWhiteOverlay, ui::playArea);
	updateDisplay();

	clock.reset();
	while (clock.elapsed_ms().count() < timeStep)
		; // nothing fancy, just a delay

	// reset screen
	clearDisplay();
	drawGrid();
	ui::quitButton->draw();
	updateDisplay();

	// a bit of delay (for good measure)
	clock.reset();
	while (clock.elapsed_ms().count() < 500)
		; // yup, nothing should happen here
}


void Game::collapseSnake() {
	using namespace core;

	do {
		L_notReadyYet:
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				state = State::QUIT;
				return;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					state = State::MENU;
					return;
				}

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				ui::quitButton->handleMouse(event);
				break;

			} // switch event.type
		}

		if (ui::quitButton->isState(Button::CLICKED)) {
			state = State::MENU;
			return;
		}

		if (clock.elapsed_ms().count() < timeStep)
			goto L_notReadyYet;
		clock.reset();

		clearDisplay();
		drawGrid();
		ui::quitButton->draw();
		updateDisplay();

	} while (grid.collapseSnakeTowardsItsMiddle());
}


//--------------------------------------------------------------
// Other
//--------------------------------------------------------------


bool menu() {
	using namespace core;

	// management
	SDL_Event menuEvent;

	// general variables
	const int menuButtonWidth = Tile::size * 10;
	const int menuButtonHeight = Tile::size * 3;

	const int menuWidth = (2*menuButtonWidth + 20);

	const int menuButtonX = (getWindowWidth() - menuWidth) / 2;
	const int menuButtonY = 340;

	const int title_posx = (getWindowWidth() - ui::gTexman->getWidth(ui::txTitle))/ 2;
	const int title_posy = 170;

	// buttons
	Font buttonFont(ui::fontPath->path.c_str(), 30);
	Button playButton(buttonFont,
	                  "Play",
	                  {menuButtonX, menuButtonY, menuButtonWidth, menuButtonHeight}
	);
	Button optionsButton(buttonFont,
	                     "Options",
	                     {menuButtonX + menuWidth - menuButtonWidth, menuButtonY, menuButtonWidth, menuButtonHeight}
	);

	// fonts
	Font smallFont(ui::fontPath->path.c_str(), 15);
	auto ftVersion = smallFont.loadText("Version: " + std::to_string(GAME_VERSION_MAJOR) + "." + std::to_string(GAME_VERSION_MINOR)
	                                    + "." + std::to_string(GAME_VERSION_BUILD) + " " + GAME_VERSION_NAME, ui::RED);

	// sliders
	Font sliderFont(ui::fontPath->path.c_str(), 10);
	Slider timeStepSlider(sliderFont,
	                      ui::speedLevel,
	                      getWindowWidth()/2 - 7*Tile::size - Tile::size/2,
	                      menuButtonY + menuButtonHeight + 2*Tile::size
	);

	// initial draw
	clearDisplay();
	ui::gTexman->draw(ui::txTitle, title_posx, title_posy);

	playButton.draw();
	optionsButton.draw();
	smallFont.draw(ftVersion, 5, 5);
	timeStepSlider.draw();

	ui::quitButton->draw();
	updateDisplay();

	// event & draw loop
	while (SDL_WaitEvent(&menuEvent)) {
		switch (menuEvent.type) {
			case SDL_QUIT:
				return false;
				break;

			case SDL_KEYDOWN:
				switch (menuEvent.key.keysym.sym) {
				case SDLK_ESCAPE:
					return false;
					break;

				case SDLK_RETURN:
					return true;
					break;

				} // menuEvent.key.keysym.sym

				break;

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				playButton.handleMouse(menuEvent);
				optionsButton.handleMouse(menuEvent);
				timeStepSlider.handleMouse(menuEvent);
				ui::quitButton->handleMouse(menuEvent);
				break;
		}

		if (playButton.isState(Button::CLICKED))
			return true;

		if (ui::quitButton->isState(Button::CLICKED))
			return false;

		// draw
		clearDisplay();
		ui::gTexman->draw(ui::txTitle, title_posx, title_posy);

		playButton.draw();
		optionsButton.draw();
		smallFont.draw(ftVersion, 5, 5);
		timeStepSlider.draw();

		ui::quitButton->draw();
		updateDisplay();
	}

	throw Failure("menu() ended at end of function (should NEVER happen)");
}
