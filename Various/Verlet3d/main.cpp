#include <onidev.h>
#include <onidev/math/math.h>
#include <onidev/window/framerate.h>
#include <functional>
#include <gl/glu.h>
#include <cmath>
#include "make_unique.h"
#include "render.h"

int main()
{
    using namespace od;
    Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, (double)640/480, 1, 1000);
    
    Verlet verlet(Vec3(0, 0, 0.2f), 0.99f, 0.8f);
    verlet.setBounds(-50, -50, -50, 50, 50, 0);
    
    // cube
    auto cube = std::make_unique<Composite>();
    auto base = cube->addParticle(Vec3(0, 0, -10));
    {
        Vec3 p(0, 0, -20);
        float r = 4;
        
        auto p000 = cube->addParticle(p + Vec3(-r, -r, -r));
        auto p100 = cube->addParticle(p + Vec3( r, -r, -r));
        auto p010 = cube->addParticle(p + Vec3(-r,  r, -r));
        auto p110 = cube->addParticle(p + Vec3( r,  r, -r));
        auto p001 = cube->addParticle(p + Vec3(-r, -r,  r));
        auto p101 = cube->addParticle(p + Vec3( r, -r,  r));
        auto p011 = cube->addParticle(p + Vec3(-r,  r,  r));
        auto p111 = cube->addParticle(p + Vec3( r,  r,  r));
        
        // Segments
        cube->addConstraint<DistanceConstraint>(p000, p100, 1);
        cube->addConstraint<DistanceConstraint>(p100, p110, 1);
        cube->addConstraint<DistanceConstraint>(p110, p010, 1);
        cube->addConstraint<DistanceConstraint>(p010, p000, 1);
        
        cube->addConstraint<DistanceConstraint>(p001, p101, 1);
        cube->addConstraint<DistanceConstraint>(p101, p111, 1);
        cube->addConstraint<DistanceConstraint>(p111, p011, 1);
        cube->addConstraint<DistanceConstraint>(p011, p001, 1);
        
        cube->addConstraint<DistanceConstraint>(p000, p001, 1);
        cube->addConstraint<DistanceConstraint>(p100, p101, 1);
        cube->addConstraint<DistanceConstraint>(p110, p111, 1);
        cube->addConstraint<DistanceConstraint>(p010, p011, 1);
        
        // Faces
        cube->addConstraint<DistanceConstraint>(p000, p110, 1);
        cube->addConstraint<DistanceConstraint>(p010, p100, 1);
        cube->addConstraint<DistanceConstraint>(p001, p111, 1);
        cube->addConstraint<DistanceConstraint>(p011, p101, 1);
        
        cube->addConstraint<DistanceConstraint>(p000, p011, 1);
        cube->addConstraint<DistanceConstraint>(p010, p001, 1);
        cube->addConstraint<DistanceConstraint>(p100, p111, 1);
        cube->addConstraint<DistanceConstraint>(p110, p101, 1);
        
        cube->addConstraint<DistanceConstraint>(p000, p101, 1);
        cube->addConstraint<DistanceConstraint>(p100, p001, 1);
        cube->addConstraint<DistanceConstraint>(p010, p111, 1);
        cube->addConstraint<DistanceConstraint>(p110, p011, 1);
        
        cube->addConstraint<DistanceConstraint>(base, p000, 1);
    }
    verlet.addComposite( std::move(cube) );
    
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 100, -50, 0, 0, 0, 0, 0,-1);
        glRotated(win.mouse_x()/2.f, 0,0,1);
        
        static float z = -50;
        if(keyCheck(vk_left)) base->pos.x -= 1;
        if(keyCheck(vk_right)) base->pos.x += 1;
        if(keyCheck(vk_up)) base->pos.y -= 1;
        if(keyCheck(vk_down)) base->pos.y += 1;
        if(keyCheck(vk_subtract)) z--;
        if(keyCheck(vk_add)) z++;
        
        base->pos.z = z;
        verlet.update(1, 14);
        draw(verlet);
        
        int r = 50;
        
        glBegin(GL_LINE_STRIP);
            glVertex3f(-r, -r, 0);
            glVertex3f(-r,  r, 0);
            glVertex3f( r,  r, 0);
            glVertex3f( r, -r, 0);
            glVertex3f(-r, -r, 0);
        glEnd();
        
        win.screenRefresh();
    }
    
    return 0;
}
