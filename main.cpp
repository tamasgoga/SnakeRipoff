/** Snake clone, by Goga Tamas */

#include "core/core_base.hpp"

#include "game/ui_globals.hpp"
#include "game/ui_color.hpp"
#include "game/ui_button.hpp"
#include "game/g_main.hpp"

#include <iostream>
#include <memory>


void noCoreMain() {
	using namespace core;

	// subsystem
	int flags = getSubsystemFlags();
	printf("Subsystems: 0x%X (%d)\n", flags, flags);
	std::cout << std::endl;

	// check flags individually:
	std::cout << "Video          " << (isSubsystemFlagOn(INIT_VIDEO)          ? "ON" : "OFF") << '\n'
	          << "Audio          " << (isSubsystemFlagOn(INIT_AUDIO)          ? "ON" : "OFF") << '\n'
	          << "Events         " << (isSubsystemFlagOn(INIT_EVENTS)         ? "ON" : "OFF") << '\n'
	          << "Timer          " << (isSubsystemFlagOn(INIT_TIMER)          ? "ON" : "OFF") << '\n'
	          << "Joystick       " << (isSubsystemFlagOn(INIT_JOYSTICK)       ? "ON" : "OFF") << '\n'
	          << "Haptic         " << (isSubsystemFlagOn(INIT_HAPTIC)         ? "ON" : "OFF") << '\n'
	          << "Gamecontroller " << (isSubsystemFlagOn(INIT_GAMECONTROLLER) ? "ON" : "OFF") << "\n\n"
	          << "Everything     " << (isSubsystemFlagOn(INIT_EVERYTHING)     ? "YES" : "NO") << std::endl;
	;

	// directorues
	std::cout << "\nApp:  " << getAppDir() << '\n';
	std::cout << "Save: " << (getSaveDir() == nullptr ? "n/a" : getSaveDir()) << std::endl;
}


bool core::init() noexcept {
	// flags
	toggleSubsystemFlagsOff(INIT_AUDIO);

	// management
	setAppInfo("gogatamas", "Snake");
	setNoCoreFn(noCoreMain);

	// window attributes
	setWindowTitle("Let's Rip Off: Snake");
	setWindowSize(800, 600);

	// initialize the screen coordinates of the grid
	const int windowWidth = getWindowWidth();
	const int windowHeight = getWindowHeight();
	ui::playArea.w = ui::playArea.h = std::min(windowWidth, windowHeight);

	if (windowWidth >= windowHeight) {
		ui::playArea.x = (windowWidth - windowHeight) / 2;
		ui::playArea.y = 0;
	} else {
		ui::playArea.x = 0;
		ui::playArea.y = (windowHeight - windowWidth) / 2;
	}

	ui::tileSize = ui::playArea.w / Grid::TILES_IN_ROW;
	ui::speedLevel = 5;

	return true;
}


void core::main() {
	using namespace std;

	// smart pointers
	auto pFontPath = make_unique<AppDirPath>("res/ponde___.ttf");
	auto pSmallButtonFont = make_unique<Font>(pFontPath->path.c_str(), 10);
	auto pQuitButton = make_unique<Button>(
		Button(
			*pSmallButtonFont,
			"x",
			{getWindowWidth() - ui::tileSize - 10, 10, ui::tileSize, ui::tileSize}
		)
	);
	auto pTexman = make_unique<core::Texman>();

	// assign smart pointers to non-owning globals
	ui::fontPath = pFontPath.get();
	ui::smallButtonFont = pSmallButtonFont.get();
	ui::quitButton = pQuitButton.get();
	ui::gTexman = pTexman.get();

	// other stuff
	ui::txSnake = ui::gTexman->create(ui::tileSize - 2, ui::tileSize - 2, ui::BLUE.r, ui::BLUE.g, ui::BLUE.b);
	ui::txSimpleFood = ui::gTexman->create(ui::tileSize - 4, ui::tileSize - 4, ui::RED.r , ui::RED.g, ui::RED.b);
	ui::txHelpBackground = ui::gTexman->create(ui::tileSize - 2, ui::tileSize - 2, ui::GRAY.r, ui::GRAY.g, ui::GRAY.b);

	// main loop
	while (showMenu()) {
		Game game;
		if (!game.run(30000))
			break;
	}
}
