#include "tile.h"

Tile::Tile(int x, int y, int h)
{
    this->x = x;
    this->y = y;
    height = h;
    heightWithWater = h;
}
