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

bool Player::placeFree(float x, float y) const
{
    return tileFree(x, y);
}

void Player::step(float dt)
{
    using namespace od;
    vy += 0.2;
    
    float& x = pos.x;
    float& y = pos.y;
    
    if(!placeFree(x, y+1))
        vy = 0;
    
    double forceX=vx;
    
    if(keyCheck(vk_left))  forceX -= 2;
    if(keyCheck(vk_right)) forceX += 2;
    
    if(keyPressed(vk_space) && !placeFree(x, y+1))
        vy -= 6;
    
    if(forceX > 0)
    {
        for(int i=0; i<forceX; i++)
        {
            if(placeFree(x+1, y)) x++;
            else {
                vx = 0;
                break;
            }
        }
    }
    else
    {
        for(int i=0; i<-forceX; i++)
        {
            if(placeFree(x-1, y)) x--;
            else {
                vx = 0;
                break;
            }
        }
    }
    
    if(vy > 0)
    {
        for(int i=0; i<vy; i++)
        {
            if(placeFree(x, y+1)) y++;
            else {
                vy = 0;
                break;
            }
        }
    }
    else
    {
        for(int i=0; i<-vy; i++)
        {
            if(placeFree(x, y-1)) y--;
            else {
                vy = 0;
                break;
            }
        }
    }
}

void Player::draw() const
{
    od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
}
