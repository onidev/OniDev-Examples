#include <onidev.h>
#include <onidev/window/framerate.h>
#include "particle.h"
#include "boids.h"

int main()
{
    using namespace od;
    Vec2 size(640, 480);
    
    Window win(size.x, size.y, "OniDev app");
    win.setSynchronization(true);
    
    Boids boids(size);
    
    boids.addRule<Barycenter>(128.f, 64.f, 0.05f);
    boids.addRule<RepulseAbsolute>(32.f, 0.8f);
    boids.addRule<MouseRepule>(64.f, 0.8f);
    boids.addRule<AverageDirection>(128.f, 0.1f);
    boids.addRule<PushedByEdges>(size, 50 );
    
    std::vector<od::Color> colors ={
        od::Color(255, 30, 20),
        od::Color(20, 255, 0),
        od::Color(20, 128, 255),
        od::Color(255, 255, 20),
        od::Color(255, 20, 255),
    };
    
    int number = 6 * colors.size();
    float r = 180;
    int i = 0;
    for(float a=0; a<360.f; a += 360.f / number)
    {
        float x = size.x/2.f + lendirx(r, a);
        float y = size.y/2.f + lendiry(r, a);
        Particle pt(x, y, colors[i / (number / colors.size())].toRgb());
        pt.vel = Vec2( lendirx(a+90), lendiry(a+90) ) * 3.f;
        
        boids.add(pt);
        i++;
    }
    
    framerateLimit(60);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear(0xFF000000);
        win.updateView();
        
        if(mousePressed(mb_left) || mouseCheck(mb_right))
        {
            for(int n=0; n<10; ++n)
            {
                float dx = lendirx(n * 360.f / 10);
                float dy = lendiry(n * 360.f / 10);
                Particle pt(win.mouse_x() + dx, win.mouse_y() + dy, colors[i%colors.size()].toRgb());
                pt.vel = Vec2( lendirx(i*90), lendiry(i*90) );
                boids.add(pt);
                i++;
            }
        }
        
        win.setCaption( "Boids: " + toString(i) + ", fps:" + toString((int)framerateGet()) );
        
        boids.simulate();
        
        if(!keyCheck(vk_space))
        {
            boids.updateRenderBuffer();
            boids.draw();
        }
        
        framerateUpdate();
        win.screenRefresh();
    }
    
    return 0;
}
