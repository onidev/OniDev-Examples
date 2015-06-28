#include <onidev.h>

class Block : public od::AutoIndex<od::Object2d, Block>
{
public:
    Block(float x, float y):
        od::AutoIndex<od::Object2d, Block>(x, y) {}
    
    od::Shape * collisionMask(int p) const {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void draw() const {
        glColor3f(1, 1, 1);
        od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16, false);
    }
};


class Player : public od::AutoIndex<od::Object2d, Player>
{
public:
    Player(float x, float y):
        od::AutoIndex<od::Object2d, Player>(x, y) {}
    
    od::Shape * collisionMask(int p) const {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void step(float dt);
    void draw() const {
        glColor3f(0, 0.5, 0);
        od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
    }
};

void Player::step(float dt)
{
    using namespace od;
    if(keyCheck(vk_right)&& !placeMeeting<Block>(pos.x+dt, pos.y)) pos.x += dt;
    if(keyCheck(vk_left) && !placeMeeting<Block>(pos.x-dt, pos.y)) pos.x -= dt;
    if(keyCheck(vk_down) && !placeMeeting<Block>(pos.x, pos.y+dt)) pos.y += dt;
    if(keyCheck(vk_up)   && !placeMeeting<Block>(pos.x, pos.y-dt)) pos.y -= dt;
}



int main()
{
    od::Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    od::instanceCreate<Player>(16*2, 16*2);
    
    for(int i=0; i<640; i+=16)
    {
        od::instanceCreate<Block>(i, 0);
        od::instanceCreate<Block>(i, 480 - 16);
    }
    for(int j=16; j<480; j+=16)
    {
        od::instanceCreate<Block>(0, j);
        od::instanceCreate<Block>(640 - 16, j);
    }
    
    for(int i=16*3; i<640; i+=16*3)
    for(int j=16*3; j<480; j+=16*3)
        od::instanceCreate<Block>(i, j-8);
    
    od::framerateSetDelta(60);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(od::keyPressed(od::vk_escape))
            break;
        
        od::drawClear();
        win.updateView();
        
        od::InstanceContext<od::Object2d>::getGlobal().update( od::deltaTime() );
        od::InstanceContext<od::Object2d>::getGlobal().render();
        
        win.screenRefresh();
        od::framerateUpdate();
    }
    
    return 0;
}
