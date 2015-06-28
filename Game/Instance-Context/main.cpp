#include <onidev.h>
#include "world.h"
#include "player.h"

int main()
{
    od::Window win(640, 480, "Instance Context Example");
    win.setSynchronization(true);
    
    glClearColor(0.3, 0.3, 0.3, 0);
    
    od::InstanceContext<od::Object2d> world1;
    od::InstanceContext<od::Object2d> world2;
    
    auto tilemap1 = std::make_shared<World>(640/16, 480/16, 48, 48);
    world1.addLayer(tilemap1);
    tilemap1->set(2, 2, true);
    
    auto tilemap2 = std::make_shared<World>(640/16, 480/16, 64, 64);
    world2.addLayer(tilemap2);
    tilemap2->set(3, 2, true);
    
    
    world1.setGlobal();
    od::instanceCreate<Player>(48, 48);
    
    world2.setGlobal();
    od::instanceCreate<Player>(64, 64);
    
    od::framerateSetDelta(60);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(od::keyPressed(od::vk_escape))
            break;
        
        od::drawClear();
        win.updateView();
        
        if(od::keyCheck(od::vk_space))
            world1.update( od::deltaTime() );
        else
            world2.update( od::deltaTime() );
        
        glBlendFunc(GL_ONE, GL_ONE);
        
        glColor3f(0.3, 0.2, 1);
        world1.render();
        tilemap1->draw();
        
        glColor3f(0.3, 0.7, 0);
        world2.render();
        tilemap2->draw();
        
        win.screenRefresh();
        od::framerateUpdate();
    }
    
    return 0;
}
