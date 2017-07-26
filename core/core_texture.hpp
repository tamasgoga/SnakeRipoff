#ifndef FLAG_CORE_CORE_TEXTURE_HPP
#define FLAG_CORE_CORE_TEXTURE_HPP

//------------------------------------------------------------
// Texture manager
//------------------------------------------------------------


#include "core_def.hpp"
#include <vector>

struct SDL_Texture;
struct SDL_Rect;
struct SDL_Surface;


namespace core {

	/** Non-inheritable random access container.
		Manages the lifetime (owns) any number of textures limited by memory. */
	class Texman {
	public:
		/** Holds information about a texture. Not very useful on it's own. */
		struct Texture {
			SDL_Texture* texture;
			int width, height;
		};

		/** Acts as both size information and index type for Texmen. (Ha! It's plural.) */
		using texindex = std::vector<Texture>::size_type;


	private:
		std::vector<Texture> textures;        // vector containing the textures
		texindex             firstEmptySlot;  // used internally to fill in gaps left after deletion

		/** Inserts the texture into the first empty slot.
		    The texture is deallocated if insertion fails. */
		texindex insert(Texture&&);


	public:
		/** Construction & destruction */
		Texman() noexcept;
		~Texman();
		Texman(const Texman&)				= delete;  // copying and moving is forbidden
		Texman(Texman&&)					= delete;
		Texman& operator=(const Texman&)	= delete;
		Texman& operator=(Texman&&)			= delete;


		/** Loading operations THROW:
		    - LoadingFailure;
		    - Exceptions of std::vector::push_back(). */

		/** Load images from file-system into the container. */
		texindex load(const char* path);
		texindex load(const char* path, uint redKey, uint greenKey, uint blueKey);  // color-keyed textures

		/** Create a texture from a preexisting surface. */
		texindex create(SDL_Surface* surface);

		/** Create a texture containing a plain color */
		texindex create(int width, int height, uint red, uint green, uint blue);

		/** Remove textures from the container */
		bool remove(texindex i) noexcept;


		/** Draw methods MAY THROW:
		    - floating point exceptions. */

		/** Draw the full texture at a specified position. */
		error draw(texindex i, int x, int y, float scaleAmount = 1.f) const;

		/** Draw a part of the texture at the specified position. */
		error draw(texindex i, int x, int y, const SDL_Rect& clip, float scaleAmount = 1.f) const;

		/** Draw the full texture within the specified bounding box. */
		error draw(texindex i, const SDL_Rect& boundingBox, float scaleAmount = 1.f) const;

		/** Draw a part of the texture within the specified bounding box. */
		error draw(texindex i, const SDL_Rect& boundingBox, const SDL_Rect& clip, float scaleAmount = 1.f) const;


		/** Modulate textures */
		error setAlphaMod(texindex i, uint alpha)						noexcept;
		error setColorMod(texindex i, uint red, uint green, uint blue)	noexcept;


		/** Check for the existence of a texture. */
		inline bool isTextureAt(texindex i) const noexcept {
			return i < textures.size() && textures[i].texture != nullptr;
		}

		/** Get size info about a texture. */
		int getWidth(texindex i)	const noexcept;
		int getHeight(texindex i)	const noexcept;


		/** Information about the underlying vector */
		inline texindex	size() const noexcept {return textures.size();};
	};


	/** Shorthand for Texman::texindex */
	using texindex = Texman::texindex;

} // namespace core


#endif // FLAG_CORE_CORE_TEXTURE_HPP
