#include "player.h"

Player::Player(float x, float y):
    od::AutoIndex<od::Object2d, Player>(x, y)
{
}

od::Shape * Player::collisionMask(int p) const
{
    static od::Rect rect(0, 0, 16, 16);
    return &rect;
}

void Player::step(float dt)
{
    using namespace od;
    if(keyCheck(vk_right)&& tileFree(pos.x+dt, pos.y)) pos.x += dt;
    if(keyCheck(vk_left) && tileFree(pos.x-dt, pos.y)) pos.x -= dt;
    if(keyCheck(vk_down) && tileFree(pos.x, pos.y+dt)) pos.y += dt;
    if(keyCheck(vk_up)   && tileFree(pos.x, pos.y-dt)) pos.y -= dt;
}

void Player::draw() const
{
    od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
}
