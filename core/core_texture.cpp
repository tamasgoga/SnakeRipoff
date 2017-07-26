#include "core_texture.hpp"
#include "core_error.hpp"
#include "core_util.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>

#include <string>


namespace core {

	// the app's renderer
	extern SDL_Renderer* renderer;


	//------------------------------------------------------------
	// CONSTRUCTION & DESTRUCTION
	//------------------------------------------------------------


	Texman::Texman() noexcept
		: textures()
		, firstEmptySlot(0)
	{;}


	Texman::~Texman() {
		for(auto t: textures) {
			SDL_DestroyTexture(t.texture);
		}
	}


	//------------------------------------------------------------
	// TEXTURE LOADING, CREATION, INSERION & REMOVAL
	//------------------------------------------------------------


	texindex Texman::insert(Texture&& texture) {
		texindex indexInsertedTo;

		if (firstEmptySlot < textures.size()) {
			textures[firstEmptySlot] = std::move(texture);
			indexInsertedTo = firstEmptySlot;

			for (++firstEmptySlot; firstEmptySlot < textures.size(); ++firstEmptySlot)
				if (textures[firstEmptySlot].texture == nullptr) {
					break;
				}
		} else {
			try {
				textures.push_back(texture);
				firstEmptySlot = textures.size();
				indexInsertedTo = firstEmptySlot - 1;
			} catch (...) {
				SDL_DestroyTexture(texture.texture);
				throw;
			}
		}

		return indexInsertedTo;
	}


	Texman::texindex Texman::load(const char* path) {
		SDL_Surface* surface = IMG_Load(path);

		if (surface == nullptr) {
			throw LoadingFailure("Failed to load \"" + std::string(path) + "\" :: " + IMG_GetError());
		}

		const int width = surface->w;
		const int height = surface->h;

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		if (texture == nullptr) {
			throw LoadingFailure("Failed make a texture out of \"" + std::string(path) + "\" :: " + SDL_GetError());
		}

		if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
			throw LoadingFailure("Failed to set the blend-mode of \"" + std::string(path) + "\" :: " + SDL_GetError());
		}

		return insert(Texture({texture, width, height}));
	}


	Texman::texindex Texman::load(const char* path, uint redKey, uint greenKey, uint blueKey) {
		SDL_Surface* surface = IMG_Load(path);

		if (surface == nullptr) {
			throw LoadingFailure("Failed to load \"" + std::string(path) + "\" :: " + IMG_GetError());
		}

		const int width = surface->w;
		const int height = surface->h;

		if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, redKey, greenKey, blueKey)) != 0) {
			throw LoadingFailure("Failed to color-key \"" + std::string(path) + "\" :: " + IMG_GetError());
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		if (texture == nullptr) {
			throw LoadingFailure("Failed make a texture out of \"" + std::string(path) + "\" :: " + SDL_GetError());
		}

		if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
			throw LoadingFailure("Failed to set the blend-mode of \"" + std::string(path) + "\" :: " + SDL_GetError());
		}

		return insert(Texture({texture, width, height}));
	}


	Texman::texindex Texman::create(int width, int height, uint red, uint green, uint blue) {
		SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

		if (surface == nullptr) {
			throw LoadingFailure("Failed to create a surface for Texture_" + std::to_string(width) + "x" + std::to_string(height)
			                     + std::to_string(red) + "." + std::to_string(green) + "." + std::to_string(blue) + " :: "
			                     + SDL_GetError());
		}

		if (SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, red, green, blue)) != 0) {
			throw LoadingFailure("Failed to fill the surface of Texture_" + std::to_string(width) + "x" + std::to_string(height)
			                     + std::to_string(red) + "." + std::to_string(green) + "." + std::to_string(blue)
			                     + " with the specified color :: " + SDL_GetError());
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		if (texture == nullptr) {
			throw LoadingFailure("Failed to create Texture_" + std::to_string(width) + "x" + std::to_string(height)
			                     + std::to_string(red) + "." + std::to_string(green) + "." + std::to_string(blue) + " :: "
			                     + SDL_GetError());
		}

		if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0) {
			throw LoadingFailure("Failed to set the blend-mode of Texture_" + std::to_string(width) + "x" + std::to_string(height)
			                     + std::to_string(red) + "." + std::to_string(green) + "." + std::to_string(blue) + " :: "
			                     + SDL_GetError());
		}

		return insert(Texture({texture, width, height}));
	}


	texindex Texman::create(SDL_Surface* surface) {
		if (surface == nullptr) {
			throw LoadingFailure("Cannot create texture from a null surface!");
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr) {
			throw LoadingFailure("Failed make a texture out of the specified surface :: " + std::string(SDL_GetError()));
		}

		return insert(Texture({texture, surface->w, surface->h}));
	}


	bool Texman::remove(texindex i) noexcept {
		if (i >= textures.size()) {
			return false;
		}

		SDL_DestroyTexture(textures[i].texture);
		textures[i].texture = nullptr;
		if (i < firstEmptySlot) {
			firstEmptySlot = i;
		}

		return true;
	}


	//------------------------------------------------------------
	// TEXTURE RENDERING
	//------------------------------------------------------------


	/** Draw the full texture at a specified position. */
	error Texman::draw(texindex i, int x, int y, float scaleAmount) const {
		if (i >= textures.size()) {
			return 1;
		}

		SDL_Rect bbox = scale({x, y, textures[i].width, textures[i].height}, scaleAmount);
		return SDL_RenderCopy(renderer, textures[i].texture, nullptr, &bbox);
	}


	/** Draw a part of the texture at the specified position. */
	error Texman::draw(texindex i, int x, int y, const SDL_Rect& clip, float scaleAmount) const {
		if (i >= textures.size()) {
			return 1;
		}

		SDL_Rect bbox = scale({x, y, textures[i].width, textures[i].height}, scaleAmount);
		return SDL_RenderCopy(renderer, textures[i].texture, &clip, &bbox);
	}


	/** Draw the full texture within the specified bounding box. */
	error Texman::draw(texindex i, const SDL_Rect& boundingBox, float scaleAmount) const {
		if (i >= textures.size()) {
			return 1;
		}

		SDL_Rect bbox = scale(boundingBox, scaleAmount);
		return SDL_RenderCopy(renderer, textures[i].texture, nullptr, &bbox);
	}


	/** Draw a part of the texture within the specified bounding box. */
	error Texman::draw(texindex i, const SDL_Rect& boundingBox, const SDL_Rect& clip, float scaleAmount) const {
		if (i >= textures.size()) {
			return 1;
		}

		SDL_Rect bbox = scale(boundingBox, scaleAmount);
		return SDL_RenderCopy(renderer, textures[i].texture, &clip, &bbox);
	}


	//------------------------------------------------------------
	// TEXTURE MODULATION
	//------------------------------------------------------------


	error Texman::setAlphaMod(texindex i, uint alpha) noexcept {
		if (i >= textures.size()) {
			return 1;
		}

		return SDL_SetTextureAlphaMod(textures[i].texture, alpha);
	}


	error Texman::setColorMod(texindex i, uint red, uint green, uint blue) noexcept {
		if (i >= textures.size()) {
			return 1;
		}

		return SDL_SetTextureColorMod(textures[i].texture, red, green, blue);
	}


	//------------------------------------------------------------
	// TEXTURE MODULATION
	//------------------------------------------------------------


	int Texman::getWidth(texindex i) const noexcept {
		if (isTextureAt(i)) {
			return textures[i].width;
		} else {
			return -1;
		}
	}


	int Texman::getHeight(texindex i) const noexcept {
		if (isTextureAt(i)) {
			return textures[i].height;
		} else {
			return -1;
		}
	}

} // namespace core
