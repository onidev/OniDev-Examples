#ifndef VERLET_PARTICLE_H_INCLUDED
#define VERLET_PARTICLE_H_INCLUDED

#include <onidev/math/vec3.h>

struct Particle
{
    od::Vec3 pos;
    od::Vec3 lastPos;
    
    Particle(od::Vec3 pos):pos(pos),lastPos(pos){}
    Particle(od::Vec3 pos, od::Vec3 velocity):pos(pos),lastPos(pos - velocity){}
};

#endif
