/** Snake clone, by Goga Tamas */

/**
	Bugs:
	  (?) bug at button removal (working now, but in a hack-y way)

	Plans:
	  0.7.8:
	    > credit myself (bottom right on menu screen)
	  0.8:
	    > storing the score
	    > proper icons for buttons (exit, options, etc.)
	  0.9:
	    > sound (old-school bleeps; melody based on snake action)
	    > vicinity food generation
	  1.0 -> testing, bug-fixes, removing unused code

	Future ideas:
	  - map editor, wall tiles
	  - multiple resolutions, full-screen
	  ! NO MORE "CORE" REWORKS

	July 26, 2017 -- Trivia: Last update to this comment on "21.06.2016" (more than a year ago) :3
*/


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

	// initialize the screen coordinates of the greed
	const int windowWidth = getWindowWidth();
	const int windowHeight = getWindowHeight();

	ui::speedLevel = 5;

	ui::playArea = {
		(windowWidth - windowHeight) / 2,
		0,
		windowHeight,
		windowHeight
	};

	Tile::size = windowHeight / TILES_IN_ROW;

	return true;
}


void core::main() {
	// local
	AppDirPath titlePicPath("res/title.png");

	// smart pointers
	auto pFontPath = std::make_unique<AppDirPath>("res/ponde___.ttf");
	auto pSmallButtonFont = std::make_unique<Font>(pFontPath->path.c_str(), 10);
	auto pQuitButton = std::make_unique<Button>(
		Button(*pSmallButtonFont, "x", {10, getWindowHeight() - Tile::size - 10, Tile::size, Tile::size})
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
		if(!game.run())
			break;
	}
}


//--------------------------------------------------------------
// Version history
//--------------------------------------------------------------


/**
	:: Version 0.7 :: Classy Folded Code ::
	- reorganized the source files

	(Project resumed on July 26, 2017.)

	:: Version 0.7 :: Classy Code ::
	- more classes, I presume

	(Project resumed at the beginning of September, 2016.)

	:: Version 0.6 :: "Polish Party" ::
	- old code reorganized
	- polishing touches added
	- menus (hard coded), buttons, sliders were added

	:: Version 0.5 ::
	- first version, because no project should start from zero
	  (plus, a most of the Core library was done)
	- a functioning, rough prototype of the game

	(Project started in the 2nd quarter of 2016.)
*/

