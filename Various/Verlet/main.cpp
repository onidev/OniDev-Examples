#include <onidev.h>
#include <onidev/math/math.h>
#include <onidev/window/framerate.h>
#include <functional>
#include "make_unique.h"
#include "render.h"
#include "examples.h"

int main()
{
    using namespace od;
    Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    Verlet verlet(Vec2(0, 0.2f), 0.99f, 0.8f);
    verlet.setBounds(0, 0, 640, 480);
    
    createSolidBlob(verlet, 320 + 320/2.f, 240, 48);
    createTree(verlet, 320, 480);
    createRope(verlet, 160, 0, 20, 10);
    createCloth(verlet, 320, 0, 25, 16, 10);
    
    std::weak_ptr<Particle> grabed;
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        
        Vec2 mouse(View::mouse_x(), View::mouse_y());
        
        if(mousePressed(mb_left))
        {
            grabed = verlet.nearestParticle(mouse);
        }
        else if(mouseReleased(mb_left))
        {
            grabed.reset();
        }
        else if(mouseCheck(mb_left) && !grabed.expired())
        {
            grabed.lock()->pos = mouse;
        }
        
        verlet.update(1, 10);
        draw(verlet);
        
        win.screenRefresh();
    }
    
    return 0;
}
