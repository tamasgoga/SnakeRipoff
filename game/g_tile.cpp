#include "g_tile.hpp"


Tile::Tile() noexcept
	: entity(NONE)
{;}


Tile::Tile(Entity entity) noexcept
	: entity(entity)
{;}
