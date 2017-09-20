#include "game.hpp"


/** Get a lighter or darker (factor < 0) shade of a color
    Doesn't change the alpha */
static SDL_Color tint(const SDL_Color& color, float factor) {
	bool lighten = true;

	if (factor < 0) {
		lighten = false;
		factor = -factor;
	}

	if (lighten) {
		return {
			uint8_t(color.r + (255 - color.r) * factor),
			uint8_t(color.g + (255 - color.g) * factor),
			uint8_t(color.b + (255 - color.b) * factor),
			color.a
		};
	}

	factor = 1.f + factor;
	return {
		uint8_t(color.r * factor),
		uint8_t(color.g * factor),
		uint8_t(color.b * factor),
		color.a
	};
}


/** Changes num in place, and returns a string containing the digits where they changed and spaces otherwise
    Sample return value: " 5 12"
    Assumes that num.size() is at least 5 */
static std::string set5DigitNum(std::string& num, int value) {
	auto changed = std::string("     ");
	value = std::abs(value);

	for (int i = 4; i >= 0; --i) {
		auto digit = static_cast<char>('0' + (value % 10));

		if (num[i] != digit)
			num[i] = changed[i] = digit;

		value /= 10;
	}

	return changed;
}


//--------------------------------------------------------------
// Class: Game
//--------------------------------------------------------------


Pulse::Pulse(uint64_t max, uint64_t initDecay, tstep_t timeStep)
	: max(max)
	, initDecay(initDecay)
	, timeStep(timeStep)
	, decay(initDecay)
	, value(0)
	, timer(true)
{;}


void Pulse::reset() {
	value = max;
	decay = initDecay;
	timer.reset();
}


uint64_t Pulse::get() {
	uint64_t decrease = decay * (timer.elapsed_ms().count() / timeStep);

	if (decrease > 0) {
		decay += (decay >> 2);
		value = value >= decrease ? value - decrease : 0;
		timer.reset();
	}

	return value;
}


//--------------------------------------------------------------
// Class: Game
//--------------------------------------------------------------


Game::Game()
	: grid()
	, texman()
	, font(ui::fontPath->path.c_str(), 20)
	, scoreText("00000")
	, clock(true)
	, state(State::PLAYING)
{
	using namespace core;

	const int window_w = getWindowWidth();
	const int window_h = getWindowHeight();

	txBlackOverlay = texman.create(window_w, window_h, 0,0,0);
	texman.setAlphaMod(txBlackOverlay, 170);
	txWhiteOverlay = texman.create(window_w, window_h, 255, 255, 255);

	ftScoreTitle = font.loadText("SCORE", ui::WHITE);
	ftScoreNumber = font.loadText(scoreText, ui::WHITE);
	ftScoreNumberOverlay = font.loadText("     ", ui::BLUE);
	ftFrateTitle = font.loadText("TSTEP", ui::WHITE);
	ftFrateNumber = font.loadText("?", ui::WHITE);
	ftPaused = font.loadText("PAUSED", ui::WHITE);
	ftGameOver = font.loadText("GAME OVER", ui::WHITE);

	font.setAlphaMod(ftScoreTitle, 180);
	font.setAlphaMod(ftFrateTitle, 180);

	int midWindow_w = window_w / 2;
	int midWindow_h = window_h / 2;

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

	// pulse score color if increased
	static Pulse scoreHighlight(255, 40, 50);
	static auto pulseColor = tint(ui::BLUE, -0.3);

	auto oldScore = grid.getScore();
	state = grid.advanceState();
	auto newScore = grid.getScore();

	// the score
	bool redrawScoreHighlight = false;
	if (newScore != oldScore) {
		font.changeText(ftScoreNumberOverlay, set5DigitNum(scoreText, newScore), pulseColor);
		font.setAlphaMod(ftScoreNumberOverlay, scoreHighlight.max);
		font.changeText(ftScoreNumber, scoreText, ui::WHITE);
		redrawScoreHighlight = true;
		scoreHighlight.reset();
	} else if (scoreHighlight.pulsed()) {
		redrawScoreHighlight = true;
		font.setAlphaMod(ftScoreNumberOverlay, scoreHighlight.get());
	}

	font.changeText(ftFrateNumber, std::to_string(timeStep), ui::WHITE);

	/// RENDERING ///

	clearDisplay();
	drawGrid(redrawScoreHighlight);
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


void Game::drawGrid(bool scoreChanged) {
	grid.draw();
	core::drawRect(ui::playArea, ui::WHITE.r, ui::WHITE.g, ui::WHITE.b);

	if (core::getWindowWidth() >= core::getWindowHeight()) {
		font.draw(ftScoreTitle, 10, 10);
		font.draw(ftScoreNumber, 10, 30);
		if (scoreChanged)
			font.draw(ftScoreNumberOverlay, 10, 30);
		font.draw(ftFrateTitle, 10, 80);
		font.draw(ftFrateNumber, 10, 100);
	} else {
		const int bottom_y = ui::playArea.y - font.getHeight(ftScoreNumber);
		const int top_y = bottom_y - font.getHeight(ftScoreTitle);
		const int second_x = font.getWidth(ftScoreTitle) + font.getWidth(ftScoreNumber) + 20;

		font.draw(ftScoreTitle, 10, top_y);
		font.draw(ftScoreNumber, 10, bottom_y);
		if (scoreChanged)
			font.draw(ftScoreNumberOverlay, 10, bottom_y);
		font.draw(ftFrateTitle, second_x, top_y);
		font.draw(ftFrateNumber, second_x, bottom_y);
	}
}


void Game::drawGameOver() {
	font.changeText(ftScoreNumber, scoreText, ui::WHITE);

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
	while (clock.elapsed_ms().count() < 1000)
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
	Button playButton(
		buttonFont,
		"Play",
		{menuButtonX, menuButtonY, menuButtonWidth, menuButtonHeight}
	);
	Button optionsButton(
		buttonFont,
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
