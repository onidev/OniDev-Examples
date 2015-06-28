#include "world.h"

World::World(int wid, int hei, float px, float py):
    wid(wid),
    hei(hei),
    tiles(wid*hei, false)
{
    // Set collision map
    for(int i=0; i<wid; ++i)
    {
        tiles[i] = true;
        tiles[i + wid * (hei-1)] = true;
    }
    for(int j=0; j<hei; ++j)
    {
        tiles[j*wid] = true;
        tiles[wid-1 + j*wid] = true;
    }
}

bool World::tileSolid(int x, int y) const
{
    return tiles[x + y * wid];
}

od::Shape * World::tileCollisionMask(int x, int y) const
{
    if(tiles[x + y * wid])
    {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    return nullptr;
}

int World::width() const
{
    return wid;
}

int World::height() const
{
    return hei;
}

int World::tileWidth() const
{
    return 16;
}

int World::tileHeight() const
{
    return 16;
}

void World::set(int x, int y, bool val)
{
    tiles[x + y*wid] = val;
}

void World::draw() const
{
    for(int i=0; i<wid; ++i)
    for(int j=0; j<hei; ++j)
    {
        if(tiles[i+j*wid])
        {
            int x = xoffset() + i * 16, y = yoffset() + j * 16;
            od::drawRectangle(x, y, x+16, y+16);
        }
    }
}
