#include "core_base.hpp"

#include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>


//------------------------------------------------------------
// Internal vars
//------------------------------------------------------------


static int         windowWidth         = 720;      // height of the window; can only be set when Core isn't running
static int         windowHeight        = 480;      // width of the window; same as above
static std::string windowTitle         = "Window"; // title of the window; same as above

static bool        run_core            = true;     // whether the engine's subsystems should be initialized or not
static bool        running             = false;    // whether the Core is running or not

static char*       devName             = nullptr;  // name of developer (used in creating save directory)
static char*       appName             = nullptr;  // name of application (same as above)
static char*       appDir              = nullptr;  // path to application
static char*       saveDir             = nullptr;  // path to save/preference files

static bool        shouldCreateSaveDir = false;    // whether to create a save/preferences directory or not

// SDL init flags w/ default value
static int sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS;
// function called if core isn't initialized
static void (*nocorefn) (void) = nullptr;


namespace core {

	// SDL initialization flags
	const int INIT_VIDEO          = SDL_INIT_VIDEO;           // video subsystem; implies INIT_EVENTS
	const int INIT_AUDIO          = SDL_INIT_AUDIO;           // audio subsystem
	const int INIT_EVENTS         = SDL_INIT_EVENTS;          // events subsystem
	const int INIT_TIMER          = SDL_INIT_TIMER;           // timer subsystem
	const int INIT_JOYSTICK	      = SDL_INIT_JOYSTICK;        // joystick subsystem
	const int INIT_HAPTIC         = SDL_INIT_HAPTIC;          // haptic (force feedback) subsystem
	const int INIT_GAMECONTROLLER = SDL_INIT_GAMECONTROLLER;  // controller subsystem; implies SDL_INIT_JOYSTICK
	const int INIT_EVERYTHING     = SDL_INIT_EVERYTHING;      // all of the above subsystems

	// window and renderer
	SDL_Window*	  window   = nullptr;  // the one and only window of the application
	SDL_Renderer* renderer = nullptr;  // the one and only renderer of the application

	// command-line arguments
	std::vector<std::string> args;


	//------------------------------------------------------------
	// Flow control
	//------------------------------------------------------------


	void quit() noexcept {
		if (!::running)
			return;

		::running = false;

		SDL_free(appDir);
		SDL_free(saveDir);

		SDL_DestroyRenderer(core::renderer);
		SDL_DestroyWindow(core::window);

		core::renderer = nullptr;
		core::window = nullptr;

		TTF_Quit();
		// IMG_Quit();
		SDL_Quit();
	}


	bool isRunning() noexcept {
		return running;
	}


	//------------------------------------------------------------
	// Getters & setters
	//------------------------------------------------------------


	int getWindowWidth() noexcept {
		return windowWidth;
	}


	int getWindowHeight() noexcept {
		return windowHeight;
	}


	std::string getWindowTitle() noexcept {
		return windowTitle;
	}


	CORE_INIT void toggleSubsystemFlagsOn(int flags) noexcept {
		if (!running) sdlFlags |=  flags;
	}


	CORE_INIT void toggleSubsystemFlagsOff(int flags) noexcept {
		if (!running) sdlFlags &= ~flags;
	}


	bool isSubsystemFlagOn(int flag) noexcept {
		return ((sdlFlags & flag) == flag);
	}


	int getSubsystemFlags() noexcept {
		return sdlFlags;
	}


	CORE_INIT void setAppInfo(const char* dev, const char* app) noexcept {
		if (saveDir == nullptr) {
			devName = (char*) dev;
			appName = (char*) app;
		}
	}


	CORE_INIT void createSaveDir(bool val) noexcept {
		::shouldCreateSaveDir = val;
	}


	CORE_INIT void setWindowSize(int width, int height) noexcept {
		if (::running)
			SDL_SetWindowSize(window, width, height);
		::windowWidth = width;
		::windowHeight = height;
	}


	CORE_INIT void setWindowTitle(std::string title) noexcept {
		if (::running)
			SDL_SetWindowTitle(window, title.c_str());
		::windowTitle = title;
	}


	CORE_INIT void setNoCoreFn(void (*ncf) (void)) noexcept {
		if (!::running)
			::nocorefn = ncf;
	}


	CORE_INIT void doNotInitCore() noexcept {
		if (!::running)
			::run_core = false;
	}



	const char* getAppDir()  {
		return appDir;
	}


	const char* getSaveDir() {
		return saveDir;
	}


	//------------------------------------------------------------
	// Classes
	//------------------------------------------------------------


	AppDirPath::AppDirPath(const char* relative)
		: path(appDir + std::string(relative))
	{;}


	SaveDirPath::SaveDirPath(const char* relative)
		: path(saveDir == nullptr ? "" : saveDir + std::string(relative))
	{
		if (saveDir == nullptr)
			throw SaveDirFailure("Cannot create a core::SaveDirPath if a Save Directory doesn't exist.");
	}

} // namespace core


//------------------------------------------------------------
// Main
//------------------------------------------------------------


static bool init_env() noexcept {
	::running = true;

	// init SDL
	if (SDL_Init(sdlFlags) != 0) {
		core::sdlErrorMessage("Failed to initialize SDL.");
		return false;
	}

	// init SDL_image
	/*if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		core::imgErrorMessage("Failed to initialize SDL_image.");
		return false;
	}*/

	// init SDL_ttf
	if (TTF_Init() != 0) {
		core::ttfErrorMessage("Failed to initialize SDL_ttf.");
		return false;
	}

	// init window
	core::window = SDL_CreateWindow(windowTitle.c_str(),
	                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight,
	                                SDL_WINDOW_SHOWN);
	if (core::window == nullptr) {
		core::sdlErrorMessage("Failed to create window");
		return false;
	}

	// init renderer
	core::renderer = SDL_CreateRenderer(core::window, -1, SDL_RENDERER_ACCELERATED);
	if (core::renderer == nullptr) {
		core::sdlErrorMessage("Failed to create renderer");
		return false;
	}

	return true;
}


int main(int argc, char** argv) {
	// user's init
	if (!core::init())
		return -1;

	// application directory path
	appDir = SDL_GetBasePath();
	if (appDir == nullptr) {
		core::sdlErrorMessage("Unable to get the application's BASE directory.");
		return -2;
	}

	// save directory path
	if (::shouldCreateSaveDir) {
		if (devName == nullptr)
			core::errorMessage("Developer name not specified, use core::setAppInfo().");
		if (appName == nullptr)
			core::errorMessage("Application name not specified, use core::setAppInfo().");

		saveDir = SDL_GetPrefPath(devName, appName);
		if (saveDir == nullptr) {
			core::sdlErrorMessage("Unable to get the application's SAVE directory.");
			return -2;
		}
	}

	try {
		for (int i = 0; i < argc; ++i) {
			core::args.push_back((const char*) argv[i]);

			// handle "nocore"
			if (core::args[i].compare("nocore") == 0)
				::run_core = false;
		}

		// init & run engine
		if (::run_core) {
			if (::init_env())
				core::main();
		// run nocore function
		} else if (::nocorefn != nullptr) {
			::nocorefn();
		}

	// various fatal exceptions
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	} catch (...) {
		std::cout << "WEIRD EXCEPTION: A really weird exception happened!" << std::endl;
	}

	core::quit();
	return 0;
}
