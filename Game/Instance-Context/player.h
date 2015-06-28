#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <onidev.h>

class Player : public od::AutoIndex<od::Object2d, Player>
{
public:
    Player(float x, float y);
    od::Shape * collisionMask(int p) const;
    void step(float dt);
    void draw() const;
};

#endif // PLAYER_H_INCLUDED
