#include "rules.h"
#include <onidev/window/view.h>

Barycenter::Barycenter(float min_distance, float max_distance, float coeff):
    min_distance_squared(min_distance * min_distance),
    max_distance_squared(max_distance * max_distance),
    coeff(coeff)
{
}

od::Vec2 Barycenter::getVelocity(Particle & pt, std::vector<Particle> & boids)
{
    od::Vec2 p(0, 0);
    int count = 0;
    
    for(auto & b: boids)
    {
        if(&b != &pt && b.color == pt.color)
        {
            if( (b.pos - pt.pos).lengthSquared() < min_distance_squared )
            {
                p += b.pos;
                count++;
            }
        }
    }
    
    if(count <= 0)
        return od::Vec2(0.f, 0.f);
    
    p /= (float)count;
    
    return (p - pt.pos) * coeff;
}


Repulse::Repulse(float min_distance, float coeff):
    min_distance_squared(min_distance * min_distance),
    coeff(coeff)
{
}

od::Vec2 Repulse::getVelocity(Particle & pt, std::vector<Particle> & boids)
{
    od::Vec2 p(0, 0);
    for(auto & b: boids)
    {
        if(&b != &pt)
        {
            if( (b.pos - pt.pos).lengthSquared() < min_distance_squared )
            {
                p -= b.pos - pt.pos;
            }
        }
    }
    return p * coeff;
}


RepulseAbsolute::RepulseAbsolute(float min_distance, float coeff):
    min_distance_squared(min_distance * min_distance),
    coeff(coeff)
{
}

od::Vec2 RepulseAbsolute::getVelocity(Particle & pt, std::vector<Particle> & boids)
{
    od::Vec2 p(0, 0);
    for(auto & b: boids)
    {
        if(&b != &pt)
        {
            if( (b.pos - pt.pos).lengthSquared() < min_distance_squared )
            {
                od::Vec2 d = b.pos - pt.pos;
                p -= d.normalized();
            }
        }
    }
    return p * coeff;
}


MouseRepule::MouseRepule(float min_distance, float coeff):
    min_distance_squared(min_distance * min_distance),
    coeff(coeff)
{
}

od::Vec2 MouseRepule::getVelocity(Particle & pt, std::vector<Particle> & boids)
{
    od::Vec2 m(od::View::mouse_x(), od::View::mouse_y());
    
    if( m != pt.pos && (m - pt.pos).lengthSquared() < min_distance_squared )
    {
        od::Vec2 d = pt.pos - m;
        return d.normalized();
    }
    
    return od::Vec2(0, 0);
}



AverageDirection::AverageDirection(float min_distance, float coeff):
    min_distance_squared(min_distance * min_distance),
    coeff(coeff)
{
}

od::Vec2 AverageDirection::getVelocity(Particle & pt, std::vector<Particle> & boids)
{
    od::Vec2 p(0, 0);
    int count = 0;
    for(auto & b: boids)
    {
        if(&b != &pt && b.color == pt.color)
        {
            if( (b.pos - pt.pos).lengthSquared() < min_distance_squared )
            {
                p += b.vel;
                count++;
            }
        }
    }
    
    if(count <= 0)
        return od::Vec2(0.f, 0.f);
    
    p /= (float)count;
    
    return (p - pt.vel) * coeff;
}

PushedByEdges::PushedByEdges(od::Vec2 size, float margin):
    size(size),
    margin(margin)
{
}

od::Vec2 PushedByEdges::getVelocity(Particle & pt, std::vector<Particle> & boids)
{
    od::Vec2 p(0, 0);
    
    for(int i=0; i<2; ++i)
    {
        if(pt.pos[i] < margin)
        p[i] = (margin - pt.pos[i]) / 100.f;
        else if(pt.pos[i] > size[i] - margin)
            p[i] = (size[i] - margin - pt.pos[i]) / 100.f;
    }
    
    return p;
}
