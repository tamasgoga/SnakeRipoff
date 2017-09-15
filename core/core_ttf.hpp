#ifndef FLAG_CORE_CORE_TTF_HPP
#define FLAG_CORE_CORE_TTF_HPP

//------------------------------------------------------------
// True-type fonts
//------------------------------------------------------------


#include "core_texture.hpp"
#include <string>

struct _TTF_Font;
struct SDL_Color;


namespace core {

	/** For better understanding read "core_texture.hpp"! */
	class Font {
	private:
		_TTF_Font* font;
		int ptsize;
		Texman texman;


	public:
		Font(const char* path, int size);
		~Font();


		/** Load methods THROW:
		    - same as Texman's draw methods
		*/

		texindex loadText(const std::string& text, const SDL_Color& color);
		texindex changeText(texindex i, const std::string& text, const SDL_Color& color);

		bool remove(texindex i) noexcept {return texman.remove(i);}


		/** Draw methods THROW:
		    - same as Texman's draw methods
		*/

		/** Draw the full texture at a specified position. */
		inline error draw(texindex i, int x, int y, float scaleAmount = 1.f) const {
			return texman.draw(i, x, y, scaleAmount);
		}

		/** Draw a part of the texture at the specified position. */
		inline error draw(texindex i, int x, int y, const SDL_Rect& clip, float scaleAmount = 1.f) const {
			return texman.draw(i, x, y, clip, scaleAmount);
		}

		/** Draw the full texture within the specified bounding box. */
		inline error draw(texindex i, const SDL_Rect& boundingBox, float scaleAmount = 1.f) const {
			return texman.draw(i, boundingBox, scaleAmount);
		}

		/** Draw a part of the texture within the specified bounding box. */
		inline error draw(texindex i, const SDL_Rect& boundingBox, const SDL_Rect& clip, float scaleAmount = 1.f) const {
			return texman.draw(i, boundingBox, clip, scaleAmount);
		}


		/** Modulate textures */

		inline error setAlphaMod(texindex i, uint alpha) noexcept {
			return texman.setAlphaMod(i, alpha);
		}

		inline error setColorMod(texindex i, uint red, uint green, uint blue) noexcept {
			return texman.setColorMod(i, red, green, blue);
		}


		/** Size information */
		inline int getWidth(texindex i)  const noexcept {return texman.getWidth(i);}
		inline int getHeight(texindex i) const noexcept {return texman.getHeight(i);}


		/** Information about the underlying Texman */
		inline texindex	size()                  const noexcept {return texman.size();};
		inline bool     isTextureAt(texindex i)	const noexcept {return texman.isTextureAt(i);}
	};

} // namespace core


#endif // FLAG_CORE_CORE_TTF_HPP
