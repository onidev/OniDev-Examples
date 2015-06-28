#include <onidev.h>
#include "world.h"
#include "player.h"

namespace
{
const int WIN_WIDTH  = 640;
const int WIN_HEIGHT = 480;
}

int main()
{
    od::Window win(WIN_WIDTH, WIN_HEIGHT, "Platformer Example");
    win.setSynchronization(true);
    
    glClearColor(0.3, 0.3, 0.3, 0);
    
    od::InstanceContext<od::Object2d> world;
    
    auto tilemap = std::make_shared<World>(WIN_WIDTH/16, WIN_HEIGHT/16, 48, 48);
    world.addLayer(tilemap);
    
    for(int i=0; i<6; ++i)
    {
        tilemap->set(8+i, 24, 1);
        tilemap->set(16+i, 18, 1);
        tilemap->set(24+i, 12, 1);
    }

    od::instanceCreate<Player>(48, 48);
    
    od::framerateSetDelta(60);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(od::keyPressed(od::vk_escape))
            break;
        
        od::drawClear();
        win.updateView();
        
        world.update( od::deltaTime() );
        world.render();
        tilemap->draw();
        
        win.screenRefresh();
        od::framerateUpdate();
    }
    
    return 0;
}
