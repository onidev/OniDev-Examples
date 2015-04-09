#ifndef VERLET_PARTICLE_H_INCLUDED
#define VERLET_PARTICLE_H_INCLUDED

#include <onidev/math/vec2.h>

struct Particle
{
    od::Vec2 pos;
    od::Vec2 lastPos;
    
    Particle(od::Vec2 pos):pos(pos),lastPos(pos){}
    Particle(od::Vec2 pos, od::Vec2 velocity):pos(pos),lastPos(pos - velocity){}
};

#endif // VERLET_PARTICLE_H_INCLUDED
