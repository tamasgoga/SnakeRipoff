/** Snake clone, by Goga Tamas */

#include "game/game.hpp"

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
	toggleSubsystemFlagsOff(INIT_AUDIO);      // audio OFF

	// management
	setAppInfo("TerriblyMediocre", "Snake");  // "Terribly Mediocre Software" ^_^
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

	Tile::size = ui::playArea.w / TILES_IN_ROW;
	ui::speedLevel = 5;

	return true;
}


void core::main() {
	// local
	AppDirPath titlePicPath("res/title.png");

	// smart pointers
	auto pFontPath = std::make_unique<AppDirPath>("res/ponde___.ttf");
	auto pSmallButtonFont = std::make_unique<Font>(pFontPath->path.c_str(), 10);
	auto pQuitButton = std::make_unique<Button>(
		Button(
			*pSmallButtonFont,
			"x",
			{10, getWindowHeight() - Tile::size - 10, Tile::size, Tile::size}
		)
	);
	auto pTexman = std::make_unique<core::Texman>();

	// assign smart pointers to globals
	ui::fontPath = pFontPath.get();
	ui::smallButtonFont = pSmallButtonFont.get();
	ui::quitButton = pQuitButton.get();
	ui::gTexman = pTexman.get();
	ui::txTitle = pTexman->load(titlePicPath.path.c_str(), 0,0,0);
	ui::txSnake = pTexman->create(Tile::size - 2, Tile::size - 2, ui::BLUE.r, ui::BLUE.g, ui::BLUE.b);
	ui::txSimpleFood = pTexman->create(Tile::size - 4, Tile::size - 4, ui::RED.r , ui::RED.g, ui::RED.b);

	while (menu()) {
		Game game;
		if (!game.run())
			break;
	}
}
