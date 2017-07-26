#include "core_ttf.hpp"
#include "core_error.hpp"

#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_rect.h"


namespace core {

	Font::Font(const char* path, int size)
		: font(nullptr)
		, ptsize(size)
		, texman()
	{
		font = TTF_OpenFont(path, ptsize);

		if (font == nullptr) {
			throw LoadingFailure("Failed to load \"" + std::string(path) + "\" (" + std::to_string(size) + " pt) :: " + TTF_GetError());
		}
	}


	Font::~Font() {
		TTF_CloseFont(font);
	}


	texindex Font::loadText(const std::string& text, const SDL_Color& color) {
		//Render text surface
	    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	    if(surface == nullptr) {
	    	throw LoadingFailure("Failed to create text surface: \"" + text + "\" (RGB: " + std::to_string(color.r) + " "
	    	                     + std::to_string(color.g) + " " + std::to_string(color.b) + ")");
	    }

        texindex index;
        try {
        	index = texman.create(surface);
        } catch (...) {
        	SDL_FreeSurface(surface);
        	throw;
        }

        SDL_FreeSurface(surface);
        return index;
	}


	texindex Font::changeText(texindex i, const std::string& text, const SDL_Color& color) {
		remove(i);
		return loadText(text, color);
	}

} // namespace core
