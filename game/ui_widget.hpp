#ifndef SNAKE_GAME_UI_WIDGET_HPP
#define SNAKE_GAME_UI_WIDGET_HPP


//--------------------------------------------------------------
// Widget aka UI component
//--------------------------------------------------------------

struct SDL_Rect;


class Widget {
public:
	/* A widget must be drawable */
	virtual void draw() const = 0;

	/* A widget must have a bounding box */
	virtual const SDL_Rect& getBoundingBox() const noexcept = 0;

	/* D-tor */
	virtual ~Widget() {;}
};


#endif // SNAKE_GAME_UI_WIDGET_HPP
