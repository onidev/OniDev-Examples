#include "particle.h"

Particle::Particle()
{
}

Particle::Particle(float x, float y):
    pos(x, y), vel(0, 0)
{
}

Particle::Particle(float x, float y, int color):
    pos(x, y), vel(0, 0), color(color), color_to_follow(color)
{
}

Particle::Particle(float x, float y, int color, int follow):
    pos(x, y), vel(0, 0), color(color), color_to_follow(follow)
{
}


void Particle::friction(float f)
{
    for(int i=0; i<2; ++i)
    {
        if(vel[i] < 0.f)
        {
            vel[i] += f;
            if(vel[i] > 0.f)
                vel[i] = 0.f;
        }
        else if(vel[i] > 0.f)
        {
            vel[i] -= f;
            if(vel[i] < 0.f)
                vel[i] = 0.f;
        }
    }
}

void Particle::limitSpeed(float max)
{
    if(vel.lengthSquared() > max*max)
       vel -= vel/100.f;
}

void Particle::loopWorld(od::Vec2 size)
{
    for(int i=0; i<2; ++i)
    {
        if(pos[i] < 0)
            pos[i] = size[i];
        else if(pos[i] > size[i])
            pos[i] = 0;
    }
}
