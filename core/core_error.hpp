#ifndef FLAG_CORE_CORE_ERROR_HPP
#define FLAG_CORE_CORE_ERROR_HPP

//------------------------------------------------------------
// Errors & exceptions
//------------------------------------------------------------

#include <string>
#include <exception>


namespace core {

	/** Handles for all the different types of error messages */
	void errorMessage(std::string msg);     // used for library independent error messages
	void sdlErrorMessage(std::string msg);  // also displays SDL_GetError()
	void imgErrorMessage(std::string msg);  // also displays IMG_GetError()
	void ttfErrorMessage(std::string msg);  // also displays TTF_GetError();


	/** Base class for all Core exceptions */
	class Failure: public std::exception {
	protected:
		std::string msg;
	public:
		Failure(std::string msg) noexcept;
		virtual const char* what() const noexcept;
		virtual ~Failure();
	};


	/** Thrown when loading, or creating something fails. */
	class LoadingFailure: public Failure {
	public:
		LoadingFailure(std::string msg) noexcept;
		const char* what() const noexcept;
	};


	/** Thrown when the save directory doesn't exist and an operation needs it. */
	class SaveDirFailure: public Failure {
	public:
		SaveDirFailure(std::string msg) noexcept;
		const char* what() const noexcept;
	};

} // namespace core

#endif // FLAG_CORE_CORE_ERROR_HPP
