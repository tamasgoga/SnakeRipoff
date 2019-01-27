#include "g_main.hpp"
#include "ui_globals.hpp"
#include "ui_color.hpp"
#include "ui_slider.hpp"

#include "../core/core_base.hpp"
#include "../core/core_draw.hpp"
#include "../core/core_util.hpp"


namespace ui {

	core::AppDirPath* fontPath = nullptr;

	core::Font* smallButtonFont = nullptr;
	Button* quitButton = nullptr;

} // namespace ui


namespace time_step {

	constexpr int MIN = 60;
	constexpr int STEP = 20;
	constexpr int BASE = 260;


	inline int get() {
		return BASE - ui::speedLevel * STEP;
	}


	inline int isValid(int timeStep) {
		return timeStep > MIN;
	}

} // namespace time_step


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


Game::Game()
	: grid()
	, texman()
	, font(ui::fontPath->path.c_str(), 20)
	, scoreText("00000")
	, scoreHighlight(255, 5, 25)
	, scoreHighlightColor(ui::tintColor(ui::BLUE, -0.3))
	, frateHighlight(255, 5, 40)
	, frateHighlightColor(ui::tintColor(ui::RED, -0.3))
	, clock(true)
	, state(State::PLAYING)
{
	using namespace core;

	const int window_w = getWindowWidth();
	const int window_h = getWindowHeight();

	txBlackOverlay = texman.create(window_w, window_h, ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);
	texman.setAlphaMod(txBlackOverlay, 170);
	txWhiteOverlay = texman.create(window_w, window_h, 255, 255, 255);

	ftScoreTitle = font.loadText("SCORE", ui::WHITE);
	ftScoreNumber = font.loadText(scoreText, ui::WHITE);
	ftScoreNumberHighlight = font.loadText("     ", ui::BLUE);
	ftFrateTitle = font.loadText("LEVEL", ui::WHITE);
	ftFrateNumber = font.loadText(std::to_string(ui::speedLevel), ui::WHITE);
	ftFrateNumberHighlight = font.loadText(std::to_string(ui::speedLevel), ui::RED);
	ftPaused = font.loadText("PAUSED", ui::WHITE);
	ftGameOver = font.loadText("FINAL SCORE", ui::WHITE);

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

	speedUpCounter = 0;

	// initial draw
	fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);
	drawGrid();
	ui::quitButton->draw();
	updateDisplay();
}


bool Game::run(int speedUpMs) {
	using namespace core;

	while(true) {
		switch (state) {
		case State::PLAYING:
			play(speedUpMs);
			break;

		case State::PAUSED:
			pause();
			break;

		case State::OVER:
		case State::WON:
			io::ScoreFile::getInstance().save(grid.getScore());

			flashScreenAndDelay();
			collapseSnake();
			endGame();
			break;

		case State::MENU:
			return true;

		case State::QUIT:
			return false;
		}
	}
}


void Game::play(int speedUpMs) {
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

	/// GAME LOGIC ///

	const int timeStep = time_step::get();
	bool hasTicked = clock.elapsed_ms().count() >= timeStep;

	if (hasTicked) {
		speedUpCounter += timeStep;
		if (speedUpCounter >= speedUpMs && time_step::isValid(timeStep)) {
			speedUpCounter = 0;
			++ui::speedLevel;

			font.changeText(ftFrateNumberHighlight, std::to_string(ui::speedLevel), frateHighlightColor);
			font.changeText(ftFrateNumber, std::to_string(ui::speedLevel), ui::WHITE);
			frateHighlight.reset();
		}

		clock.reset();

		auto oldScore = grid.getScore();
		state = grid.advanceState();
		auto newScore = grid.getScore();

		// the score
		if (newScore != oldScore) {
			font.changeText(ftScoreNumberHighlight, set5DigitNum(scoreText, newScore), scoreHighlightColor);
			font.changeText(ftScoreNumber, scoreText, ui::WHITE);
			scoreHighlight.reset();
		}
	}

	/// RENDERING ///
	bool hasScoreHighlChanged = scoreHighlight.hasPulsed();
	bool hasFrateHighlChanged = frateHighlight.hasPulsed();

	if (hasTicked || hasScoreHighlChanged || hasFrateHighlChanged) {
		if (hasScoreHighlChanged)
			font.setAlphaMod(ftScoreNumberHighlight, scoreHighlight.get());
		if (hasFrateHighlChanged)
			font.setAlphaMod(ftFrateNumberHighlight, frateHighlight.get());

		fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);
		drawGrid();
		ui::quitButton->draw();
		updateDisplay();
	}
}


void Game::pause() {
	using namespace core;

	fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);

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

		fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);

		drawGrid();
		texman.draw(txBlackOverlay, 0, 0);
		font.draw(ftPaused, midFtPaused_w, midFtPaused_h);
		ui::quitButton->draw();

		updateDisplay();
	}
}


void Game::endGame() {
	using namespace core;

	fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);

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

		fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);

		drawGameOver();
		ui::quitButton->draw();

		updateDisplay();
	}
}


void Game::drawGrid() {
	grid.draw();
	core::drawRect(ui::playArea, ui::WHITE.r, ui::WHITE.g, ui::WHITE.b);

	// scale highlight
	// 1.f + 0.001f * (scoreHighlight.max - scoreHighlight.get())

	if (core::getWindowWidth() >= core::getWindowHeight()) {
		font.draw(ftScoreTitle, 10, 10);
		font.draw(ftScoreNumber, 10, 30);
		if (scoreHighlight.isActive())
			font.draw(ftScoreNumberHighlight, 10, 30);
		font.draw(ftFrateTitle, 10, 80);
		font.draw(ftFrateNumber, 10, 100);
		if (frateHighlight.isActive())
			font.draw(ftFrateNumberHighlight, 10, 100);
	} else {
		const int bottom_y = ui::playArea.y - font.getHeight(ftScoreNumber);
		const int top_y = bottom_y - font.getHeight(ftScoreTitle);
		const int second_x = font.getWidth(ftScoreTitle) + font.getWidth(ftScoreNumber) + 20;

		font.draw(ftScoreTitle, 10, top_y);
		font.draw(ftScoreNumber, 10, bottom_y);
		if (scoreHighlight.isActive())
			font.draw(ftScoreNumberHighlight, 10, bottom_y);
		font.draw(ftFrateTitle, second_x, top_y);
		font.draw(ftFrateNumber, second_x, bottom_y);
		if (frateHighlight.isActive())
			font.draw(ftFrateNumberHighlight, second_x, bottom_y);
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
	while (clock.elapsed_ms().count() < time_step::get())
		; // nothing fancy, just a delay

	// reset screen
	fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);
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

		if (clock.elapsed_ms().count() < time_step::get())
			goto L_notReadyYet;
		clock.reset();

		fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);
		drawGrid();
		ui::quitButton->draw();
		updateDisplay();

	} while (grid.collapseSnakeTowardsItsMiddle());
}


//--------------------------------------------------------------
// Other
//--------------------------------------------------------------


// true: play, false: quit
bool showMenu() {
	using namespace core;

	// management
	SDL_Event menuEvent;

	// fonts
	Font titleFont(ui::fontPath->path.c_str(), 40);
	Font smallFont(ui::fontPath->path.c_str(), 15);
	Font buttonFont(ui::fontPath->path.c_str(), 30);

	// general variables
	const int menuButtonWidth = ui::tileSize * 10;
	const int menuButtonHeight = ui::tileSize * 3;

	const int menuWidth = (2 * menuButtonWidth + 20);

	const int menuButtonX = (getWindowWidth() - menuWidth) / 2;
	const int menuButtonY = 280;

	// title text
	auto ftTitleSeries = titleFont.loadText("Let's Rip Off:", ui::BLUE);
	auto ftTitleGame = titleFont.loadText("Snake", ui::BLUE);
	const int titleSeries_posx = (getWindowWidth() - titleFont.getWidth(ftTitleSeries)) / 2;
	const int titleSeries_posy = 170 - 40;
	const int titleGame_posx = (getWindowWidth() - titleFont.getWidth(ftTitleGame)) / 2;
	const int titleGame_posy = titleSeries_posy + titleFont.getHeight(ftTitleSeries) + 5;

	// buttons
	Button playButton(
		buttonFont,
		"Play",
		{menuButtonX, menuButtonY, menuButtonWidth, menuButtonHeight}
	);
	Button scoresButton(
		buttonFont,
		"Scores",
		{menuButtonX + menuWidth - menuButtonWidth, menuButtonY, menuButtonWidth, menuButtonHeight}
	);

	// credits text
	auto ftVersion = smallFont.loadText("Version: " + std::to_string(GAME_VERSION_MAJOR) + "." + std::to_string(GAME_VERSION_MINOR)
	                                    + "." + std::to_string(GAME_VERSION_BUILD) + " " + GAME_VERSION_NAME, ui::WHITE);
	auto ftCredits = smallFont.loadText("by Goga Tamas", ui::WHITE);

	const int credit_posx = getWindowWidth() - smallFont.getWidth(ftCredits) - 5;
	const int credit_posy = getWindowHeight() - smallFont.getHeight(ftCredits) - 5;

	// sliders
	Font sliderFont(ui::fontPath->path.c_str(), 10);
	Slider timeStepSlider(
		sliderFont,
		ui::speedLevel,
		getWindowWidth()/2 - 7*ui::tileSize - ui::tileSize/2,
		menuButtonY + menuButtonHeight + 2*ui::tileSize + 40 + 25
	);

	// slider text
	auto ftSpeed = smallFont.loadText("-- SPEED --", ui::WHITE);
	const int speedTextX = (getWindowWidth() - smallFont.getWidth(ftSpeed)) / 2;
	const int speedTextY = timeStepSlider.getBoundingBox().y - 25;

	// draw the menu
	auto renderMenuPage = [&] () {
		fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);

		titleFont.draw(ftTitleSeries, titleSeries_posx, titleSeries_posy);
		titleFont.draw(ftTitleGame, titleGame_posx, titleGame_posy);
		playButton.draw();
		scoresButton.draw();
		smallFont.draw(ftSpeed, speedTextX, speedTextY);
		smallFont.draw(ftVersion, 5, 5);
		smallFont.draw(ftCredits, credit_posx, credit_posy);
		timeStepSlider.draw();
		ui::quitButton->draw();

		updateDisplay();
	};

	// initial draw
	renderMenuPage();

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
				scoresButton.handleMouse(menuEvent);
				timeStepSlider.handleMouse(menuEvent);
				ui::quitButton->handleMouse(menuEvent);
				break;
		}

		if (playButton.isState(Button::CLICKED))
			return true;

		if (ui::quitButton->isState(Button::CLICKED))
			return false;

		// make sure to check this last
		if (scoresButton.isState(Button::SELECTED)) {
			if (!showScores())
				return false;

			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			// unfortunately we have to manually reset this, probably a bug with the button
			scoresButton.setState(isInside(mouseX, mouseY, scoresButton.getBoundingBox()) ? Button::HOVER : Button::NORMAL);
		}

		renderMenuPage();
	}

	throw Failure("MENU ended at end of function (should NEVER happen)");
}


// returns false if SDL_QUIT
bool showScores() {
	using namespace core;

	// variables
	auto& scores = io::ScoreFile::getInstance();
	static constexpr int FONT_SIZE = 15;
	static constexpr int MIN_SCORE_BOARD_LEN = 10;
	auto getScoreBoardLength = [&scores] () {
		return scores.size() < MIN_SCORE_BOARD_LEN ? scores.size() : MIN_SCORE_BOARD_LEN;
	};

	SDL_Event event;
	Font font(ui::fontPath->path.c_str(), FONT_SIZE);

	std::vector<texindex> ftScores;
	ftScores.reserve(getScoreBoardLength());

	// title
	Font titleFont(ui::fontPath->path.c_str(), 40);
	auto ftScoreTitle = titleFont.loadText("Scoreboard", ui::YELLOW);

	// scores
	int maxTextWidth = 0;

	auto resetScoreBoard = [&scores, &ftScores, &font, &maxTextWidth, &getScoreBoardLength] () {
		ftScores.clear();

		int len = getScoreBoardLength();
		for (int i = 0; i < len; ++i) {
			ftScores.push_back(font.loadText(
				std::string(i < 9 ? " " : "") + std::to_string(i + 1) + ".   " + std::to_string(scores[i]),
				ui::WHITE
			));

			int w = font.getWidth(ftScores[i]);
			if (w > maxTextWidth) {
				maxTextWidth = w;
			}
		}
	};

	resetScoreBoard();

	const int textPosX = (getWindowWidth() - maxTextWidth) / 2;
	const int textPosY = (getWindowHeight() - ((FONT_SIZE + 1) * 11)) / 2;

	// reset button
	Font buttonFont(ui::fontPath->path.c_str(), 30);
	const int buttonWidth = ui::tileSize * 10;
	Button resetButton = Button(
		buttonFont,
		"Reset",
		{
			(getWindowWidth() - buttonWidth) / 2,
			textPosY + FONT_SIZE * (MIN_SCORE_BOARD_LEN + 3),
			buttonWidth,
			ui::tileSize * 3
		}
	);

	auto renderScoresPage = [
			&font, &resetButton, &titleFont, ftScoreTitle,
			ftScores, &getScoreBoardLength, textPosX, textPosY
		] () {
		fillDisplay(ui::BLACK.r, ui::BLACK.g, ui::BLACK.b);

		titleFont.draw(
			ftScoreTitle,
			(getWindowWidth() - titleFont.getWidth(ftScoreTitle)) / 2,
			textPosY - FONT_SIZE - titleFont.getHeight(ftScoreTitle)
		);

		int len = getScoreBoardLength();
		for (int i = 0; i < len; ++i) {
			font.draw(ftScores[i], textPosX, textPosY + i * FONT_SIZE + FONT_SIZE);
		}

		resetButton.draw();
		ui::quitButton->draw();

		updateDisplay();
	};

	// initial draw
	renderScoresPage();

	// event & draw loop
	while (SDL_WaitEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				return false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_RETURN:
					return true;
					break;
				} // event.key.keysym.sym

				break;

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
				resetButton.handleMouse(event);
				ui::quitButton->handleMouse(event);
				break;
		}

		if (resetButton.isState(Button::CLICKED)) {
			scores.reset();
			resetScoreBoard();
		}

		if (ui::quitButton->isState(Button::CLICKED)) {
			return true;
		}

		renderScoresPage();
	}

	throw Failure("SCORE page ended at end of function (should NEVER happen)");
}
