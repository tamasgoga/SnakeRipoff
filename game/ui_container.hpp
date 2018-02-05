#ifndef SNAKE_GAME_UI_CONTAINER_HPP
#define SNAKE_GAME_UI_CONTAINER_HPP


//--------------------------------------------------------------
// Container widgets with scrolling overflow
//--------------------------------------------------------------

#include "ui_widget.hpp"
#include <SDL2/SDL_rect.h>
#include <vector>


// A container should only exist until it's widgets are there
class Container: public Widget {
public:
	using container_type = std::vector<const Widget*>;
	using index = container_type::size_type;

	Container(int x, int y, int w, int h) noexcept;
	virtual ~Container();

	index add(const Widget& w);

	bool remove(index i);

	void draw() const {
		return;
	}

	inline const SDL_Rect& getBoundingBox() const noexcept {
		return box;
	}

private:
	// Bounding box
	SDL_Rect box;

	// index of the next inserted elem
	index nextIndex;

	// Non-owning pointers
	std::vector<const Widget*> container;
};


#endif // SNAKE_GAME_UI_CONTAINER_HPP
