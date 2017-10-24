#include "entities.hpp"

//--------------------------------------------------------------
// class: Tile
//--------------------------------------------------------------


Tile::Tile() noexcept
	: x(0)
	, y(0)
	, entity(NONE)
{;}


Tile::Tile(int x, int y, Entity entity) noexcept
	: x(x)
	, y(y)
	, entity(entity)
{;}


int Tile::size = 0;


//--------------------------------------------------------------
// class: Button
//--------------------------------------------------------------


Button::Button(core::Font& font, std::string text, const SDL_Rect& box)
	: box(box)
	, state(NORMAL)
	, text(text)
	, font(font)
{
	ftText = font.loadText(text, normal);
	midFtText_w = (2*box.x + box.w) / 2 - font.getWidth(ftText) / 2;
	midFtText_h = (2*box.y + box.h) / 2 - font.getHeight(ftText) / 2;
}


Button::Button(const Button& other)
	: box(other.box)
	, state(other.state)
	, text(other.text)
	, font(other.font)
{
	ftText = ftText = font.loadText(text, normal);
	midFtText_w = other.midFtText_w;
	midFtText_h = other.midFtText_h;
}


Button::Button(Button&& other)
	: box(other.box)
	, state(other.state)
	, text(other.text)
	, font(other.font)
	, ftText(other.ftText)
	, midFtText_w(other.midFtText_w)
	, midFtText_h(other.midFtText_h)
{
	// empty text signals move
	other.text.clear();
}


Button::~Button() {
	// empty text signals move
	if (!text.empty())
		font.remove(ftText);
}


void Button::draw() const {
	switch(state) {
	case NORMAL:
		core::setDrawColor(normal.r, normal.g, normal.b);
		break;
	case CLICKED:
	case HOVER:
		core::setDrawColor(hover.r, hover.g, hover.b);
		break;
	case SELECTED:
		core::setDrawColor(select.r, select.g, select.b);
		break;
	}

	core::drawRect(box);
	font.draw(ftText, midFtText_w, midFtText_h);
}


Button::ButtonState Button::handleMouse(SDL_Event& event) noexcept {
	// mouse
	int mouseX, mouseY;
	Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	if (core::isInside(mouseX, mouseY, box)) {
		switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				state = SELECTED;
			else
				state = HOVER;
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				state = CLICKED;
			else
				state = HOVER;
			break;

		case SDL_MOUSEMOTION:
			if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
				state = SELECTED;
			else
				state = HOVER;
			break;

		default:
			state = HOVER;
			break;
		}
	} else {
		state = NORMAL;
	}

	return state;
}


SDL_Color Button::normal = ui::WHITE;
SDL_Color Button::hover = ui::BLUE;
SDL_Color Button::select = ui::RED;


//--------------------------------------------------------------
// class: Slider
//--------------------------------------------------------------


Slider::Slider(core::Font& font, int& sliderVariable, int x, int y)
	: decButton(font, "-", {x, y, Tile::size, Tile::size})
	, incButton(font, "+", {x + Tile::size*13, y, Tile::size, Tile::size})
	, level(sliderVariable)
	, mousedOver(false)
{
	if (level < MIN) {
		level = MIN;
	} else if (level > MAX) {
		level = MAX;
	}
}


void Slider::handleMouse(SDL_Event& event) {
	int mx, my;
	SDL_GetMouseState(&mx, &my);

	// check if mouse is inside
	if (mx >= decButton.box.x && mx <= incButton.box.x + incButton.box.w &&
	    my >= decButton.box.y && my <= decButton.box.y + decButton.box.h)
	{
		mousedOver = true;

		// mouse-wheel event
		if (event.type == SDL_MOUSEWHEEL) {
			level += event.wheel.y;

			if (level < MIN)
				level = MIN;
			else if (level > MAX)
				level = MAX;
		}
	} else {
		mousedOver = false;
	}

	if (decButton.handleMouse(event) == Button::CLICKED) {
		dec();
	} else if (incButton.handleMouse(event) == Button::CLICKED) {
		inc();
	}
}


void Slider::draw() const {
	decButton.draw();
	int posx = decButton.box.x + 2*Tile::size;

	for (int i = 0; i < level; ++i) {
		if (mousedOver)
			ui::gTexman->draw(ui::txSimpleFood, {posx + 1, decButton.box.y + 1, Tile::size - 2, Tile::size - 2});
		else
			ui::gTexman->draw(ui::txSnake, posx + 1, decButton.box.y + 1);
		posx += Tile::size;
	}

	incButton.draw();
}


//--------------------------------------------------------------
// class: Grid
//--------------------------------------------------------------


Grid::Grid()
	: grid(), snake(), snakeHead(0), snakeSize(1)
{
	// set additional variables
	int posx, posy, curr;
	posx = ui::playArea.x;
	posy = ui::playArea.y;

	// build grid
	for (int r = 0; r < TILES_IN_ROW; ++r) {
		for (int c = 0; c < TILES_IN_ROW; ++c) {
			curr = r * TILES_IN_ROW + c;

			grid[curr].x = posx;
			grid[curr].y = posy;

			posx += Tile::size;
		}

		posx = ui::playArea.x;
		posy += Tile::size;
	}

	int i = 12;
	const int bodyLength = i + INIT_SNAKE_SIZE;

	// head
	snake[0] = 15 * TILES_IN_ROW + i;
	grid[15 * TILES_IN_ROW + i].entity = Tile::SNAKE_HEAD;

	// body
	for (i = i + 1; i < bodyLength; ++i) {
		snake[snakeSize++] = 15 * TILES_IN_ROW + i;
		grid[15 * TILES_IN_ROW + i].entity = Tile::SNAKE_BODY;
	}

	// movement
	direction = Direction::LEFT;
	currentDirection = Direction::LEFT;

	generateFood();
	score = 0;
}


// returns State::WON if no more food can be generated
State Grid::generateFood() {
	// won
	if (snakeSize == MAX)
		return State::WON;

	int freeTileCount = MAX - snakeSize;
	if (freeTileCount > TILES_IN_ROW) {
		// generate a random number
		do {
			simpleFood = core::randomInt(rng32, 0, MAX - 1);
		} while (grid[simpleFood].entity != Tile::NONE);

	} else {
		// the snake is very large, pick one of the empty spaces
		int emptyTiles[TILES_IN_ROW];
		int emptyTilesCount = 0;

		for (int i = 0; i < MAX; ++i) {
			if (grid[i].entity == Tile::NONE) {
				emptyTiles[emptyTilesCount++] = i;

				if (emptyTilesCount == TILES_IN_ROW)
					break;
			}
		}

		simpleFood = emptyTiles[core::randomInt(rng32, 0, emptyTilesCount - 1)];
	}

	grid[simpleFood].entity = Tile::SIMPLE_FOOD;

	return State::PLAYING;
}


void Grid::draw() const {
	for (auto tile: grid) {
		switch (tile.entity) {
		case Tile::SNAKE_HEAD:
			ui::gTexman->draw(ui::txSnake, tile);
			break;

		case Tile::SNAKE_BODY:
			ui::gTexman->draw(ui::txSnake, tile.x + 1, tile.y + 1);
			break;

		case Tile::SIMPLE_FOOD:
			ui::gTexman->draw(ui::txSimpleFood, tile.x + 2, tile.y + 2);
			break;

		default: break;
		}
	}
}


void Grid::handleKeyboard() {
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);

	if (currentDirection == Direction::UP || currentDirection == Direction::DOWN) {
		// normal movement
		if (keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT]) {
			direction = Direction::LEFT;
		} else if (keystates[SDL_SCANCODE_RIGHT] && !keystates[SDL_SCANCODE_LEFT]) {
			direction = Direction::RIGHT;
		}
	}

	else if (currentDirection == Direction::LEFT || currentDirection == Direction::RIGHT) {
		// normal movement
		if (keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN]) {
			direction = Direction::UP;
		} else if (keystates[SDL_SCANCODE_DOWN] && !keystates[SDL_SCANCODE_UP]) {
			direction = Direction::DOWN;
		}
	}
}


// returns a game state
State Grid::advanceState() {
	if (snakeSize == 0)
		throw core::Failure("How the fuck wasn't the snake initialized?! I had one job!!");

	// next & previous positions (obviously the head at the beginning)
	int prev = snake[0];

	// move head
	switch (direction) {
	case Direction::UP:
		if (prev < TILES_IN_ROW)
			snake[0] = (TILES_IN_ROW - 1) * TILES_IN_ROW + prev;
		else
			snake[0] = prev - TILES_IN_ROW;
		break;

	case Direction::DOWN:
		if (prev >= (TILES_IN_ROW - 1) * TILES_IN_ROW)
			snake[0] = prev % TILES_IN_ROW;
		else
			snake[0] = prev + TILES_IN_ROW;
		break;

	case Direction::LEFT:
		if (prev % TILES_IN_ROW == 0)
			snake[0] = prev + (TILES_IN_ROW - 1);
		else
			snake[0] -= 1;
		break;

	case Direction::RIGHT:
		if (prev % TILES_IN_ROW == TILES_IN_ROW - 1)
			snake[0] = prev - (TILES_IN_ROW - 1);
		else
			snake[0] += 1;
		break;

	default: break;
	}

	// check if food, move head
	bool newFoodNeeded = false;
	if (grid[snake[0]].entity == Tile::SIMPLE_FOOD) {
		score += ui::speedLevel;
		newFoodNeeded = true;
	} else if (grid[snake[0]].entity != Tile::NONE) {
		snake[0] = prev;
		return State::OVER;
	}

	grid[snake[0]].entity = Tile::SNAKE_HEAD;

	// move rest
	int temp;
	for (int i = 1; i < snakeSize; ++i) {
		temp = snake[i];
		snake[i] = prev;
		prev = temp;

		grid[snake[i]].entity = Tile::SNAKE_BODY;
	}

	// set the direction
	currentDirection = direction;

	// generate food, body parts
	if (newFoodNeeded) {
		snake[snakeSize++] = prev;
		return generateFood();
	} else {
		grid[prev].entity = Tile::NONE;
	}

	return State::PLAYING;
}


// used when killing the snake; returns false if the snake collapsed already
bool Grid::collapseSnakeTowardsItsMiddle() {
	if (snakeHead > snakeSize)
		return false;

	// collapse
	grid[snake[snakeHead]].entity = Tile::NONE;
	grid[snake[snakeSize--]].entity = Tile::NONE;

	// size matters :p
	grid[snake[++snakeHead]].entity = Tile::SNAKE_HEAD;

	return true;
}
