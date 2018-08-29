#include "core_error.hpp"

#include <SDL2/SDL_error.h>
// #include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>


namespace core {

	//------------------------------------------------------------
	// Error messages
	//------------------------------------------------------------


	void errorMessage(std::string msg) {
		std::cerr << "ERROR: " << msg << std::endl;
	}


	void sdlErrorMessage(std::string msg) {
		std::cerr << "ERROR: " << msg << "\n\tSDL: " << SDL_GetError() << std::endl;
	}


	/*void imgErrorMessage(std::string msg) {
		std::cerr << "ERROR: " << msg << "\n\tIMG: " << IMG_GetError() << std::endl;
	}*/


	void ttfErrorMessage(std::string msg) {
		std::cerr << "ERROR: " << msg << "\n\tTTF: " << TTF_GetError() << std::endl;
	}


	//------------------------------------------------------------
	// class: Failure
	//------------------------------------------------------------


	Failure::Failure(std::string msg) noexcept
		: msg(msg)
	{;}


	Failure::~Failure() {;}


	const char* Failure::what() const noexcept {
		std::string ret = "FAILURE: " + msg;
		return ret.c_str();
	}


	//------------------------------------------------------------
	// class: LoadingFailure
	//------------------------------------------------------------


	LoadingFailure::LoadingFailure(std::string msg) noexcept
		: Failure(msg)
	{;}


	const char* LoadingFailure::what() const noexcept {
		std::string ret = "LOADING FAILURE: " + msg;
		return ret.c_str();
	}


	//------------------------------------------------------------
	// class: SaveDirFailure
	//------------------------------------------------------------


	SaveDirFailure::SaveDirFailure(std::string msg) noexcept
		: Failure(msg)
	{;}


	const char* SaveDirFailure::what() const noexcept {
		std::string ret = "SAVE DIR FAILURE: " + msg;
		return ret.c_str();
	}

} // namespace core
