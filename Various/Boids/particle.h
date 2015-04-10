#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include <onidev/math/vec2.h>
#include <vector>

struct Particle
{
    od::Vec2 pos;
    od::Vec2 vel;
    int color = 0xFFFFFF;
    int color_to_follow = 0xFFFFFF;
    
    Particle();
    Particle(float x, float y);
    Particle(float x, float y, int color);
    Particle(float x, float y, int color, int follow);
    
    void friction(float f);
    void limitSpeed(float max);
    
    void loopWorld(od::Vec2 size);
};


#endif // PARTICLE_H_INCLUDED
