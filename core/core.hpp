#ifndef FLAG_CORE_CORE_HPP
#define FLAG_CORE_CORE_HPP

//------------------------------------------------------------
// Core (main header file)
//------------------------------------------------------------


/* Note! All exceptions are handled within main, after which core::quit() is called.
Any or all of them can, of course, be caught before. */

#include "core_def.hpp"
#include "core_draw.hpp"
#include "core_error.hpp"
#include "core_random.hpp"
#include "core_texture.hpp"
#include "core_time.hpp"
#include "core_ttf.hpp"
#include "core_util.hpp"

#include <vector>
#include <string>


namespace core {

	//------------------------------------------------------------
	// Application management
	//------------------------------------------------------------


	/** Subsystem initialization flags which hold the same value as their equivalent SDL_INIT_... flags.
	    VIDEO, AUDIO and EVENTS are initialized implicitly. */
	extern const int INIT_VIDEO;           // video subsystem; implies INIT_EVENTS
	extern const int INIT_AUDIO;           // audio subsystem
	extern const int INIT_EVENTS;          // events subsystem
	extern const int INIT_TIMER;           // timer subsystem
	extern const int INIT_JOYSTICK;        // joystick subsystem
	extern const int INIT_HAPTIC;          // haptic (force feedback) subsystem
	extern const int INIT_GAMECONTROLLER;  // controller subsystem; implies INIT_JOYSTICK
	extern const int INIT_EVERYTHING;      // all of the above subsystems

	/** The window of the application. */
	extern SDL_Window* window;

	/** Vector containing all command-line arguments. */
	extern std::vector<std::string> args;


	//------------------------------------------------------------
	// Flow control
	//------------------------------------------------------------


	/** Not defined:
	    This is the first function to be called. Should contain CORE_INIT calls.
	    If it returns false, nothing else is performed and the program exits with -1. */
	CORE_PROTOTYPE bool init() noexcept;

	/** Not defined:
	    Should contain data initialization and the main loop.
	    It's called after every internal variable is initialized. */
	CORE_PROTOTYPE void main();

	/** The only way to quit Core (and close the window) */
	void quit() noexcept;

	/** Check if Core is running */
	bool isRunning() noexcept;


	//------------------------------------------------------------
	// Getters & setters
	//------------------------------------------------------------


	/** Get window parameters */
	int         getWindowWidth()  noexcept;
	int         getWindowHeight() noexcept;
	std::string getWindowTitle()  noexcept;

	/** Set application info */
	CORE_INIT void setAppInfo(const char* dev, const char* app) noexcept;
	/** Specify whether the preferences/save directory will be created or not */
	CORE_INIT void createSaveDir(bool val = true) noexcept;

	/** Toggle subsystem flags. VIDEO, AUDIO and EVENTS are initialized implicitly.
	    More flags can be passed separated by logical ORing ('|') them together.
	    Has no effect outside of core::init(). */
	CORE_INIT void toggleSubsystemFlagsOn(int flags)  noexcept;
	CORE_INIT void toggleSubsystemFlagsOff(int flags) noexcept;

	/** Check whether a flag is set. */
	bool isSubsystemFlagOn(int flag) noexcept;
	/** Get the integer that stores the flags. */
	int getSubsystemFlags() noexcept;

	/** Set window parameters */
	CORE_INIT void setWindowSize(int width, int height)	noexcept;
	CORE_INIT void setWindowTitle(std::string title) 	noexcept;

	/** Calls the function passed as a parameter whenever the app is run without Core.
	    Core won't run if any of the following is true:
	      (1) "nocore" is one of the command-line arguments;
	      (2) doNotInitCore() is called in core::init().

	    Only has effect when core is not already running.
	*/
	CORE_INIT void setNoCoreFn(void (*ncf) (void)) noexcept;

	/** Disables Core. Only has effect when core is not already running. */
	CORE_INIT void doNotInitCore() noexcept;

	/** Directory paths */
	const char* getAppDir();
	const char* getSaveDir();


	//------------------------------------------------------------
	// Classes
	//------------------------------------------------------------


	/** Contains a path in the application directory. */
	struct AppDirPath {
		const std::string path;
		AppDirPath(const char* relative);
	};


	/** Contains a path in the save directory. */
	struct SaveDirPath {
		const std::string path;
		SaveDirPath(const char* relative);
	};

} // namespace core

#endif // FLAG_CORE_CORE_HPP
