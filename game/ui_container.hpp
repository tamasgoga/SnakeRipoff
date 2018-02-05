#ifndef SNAKE_GAME_UI_CONTAINER_HPP
#define SNAKE_GAME_UI_CONTAINER_HPP


//--------------------------------------------------------------
// Container widgets with scrolling overflow
//--------------------------------------------------------------

#include <SDL2/SDL_rect.h>


class Container {
public:
	Container(int x, int y, int w, int h) noexcept;
	virtual ~Container();

	void draw() {
		return;
	}

	inline const SDL_Rect& getBoundingBox() const noexcept {
		return box;
	}

private:
	SDL_Rect box;
};


#endif // SNAKE_GAME_UI_CONTAINER_HPP
