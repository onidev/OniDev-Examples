#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <onidev.h>

class World:
    public od::TileMap
{
    int wid, hei;
    std::vector<bool> tiles;
    
public:
    World(int wid, int hei, float px, float py);
    
    bool tileSolid(int x, int y) const;
    od::Shape * tileCollisionMask(int x, int y) const;
    
    int width() const;
    int height() const;
    int tileWidth() const;
    int tileHeight() const;
    
    void set(int x, int y, bool val);
    
    void draw() const;
};

#endif // WORLD_H_INCLUDED
