#include "g_tile.hpp"


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
