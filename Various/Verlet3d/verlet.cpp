#include "verlet.h"
#include <cmath>

void Verlet::setBounds(float x1, float y1, float z1, float x2, float y2, float z2)
{
    _x1 = x1;
    _y1 = y1;
    _z1 = z1;
    _x2 = x2;
    _y2 = y2;
    _z2 = z2;
    _haveBounds = true;
}

void Verlet::bounds(std::shared_ptr<Particle> & particle)
{
    if(particle->pos.x < _x1)
        particle->pos.x = _x1;
    if(particle->pos.x > _x2)
        particle->pos.x = _x2;
        
    if(particle->pos.y < _y1)
        particle->pos.y = _y1;
    if(particle->pos.y > _y2)
        particle->pos.y = _y2;
    
    if(particle->pos.z < _z1)
        particle->pos.z = _z1;
    if(particle->pos.z > _z2)
        particle->pos.z = _z2;
}

void Verlet::update(float dt, int iter)
{
    //static float lastDt = dt;
    for(auto& c: _composites)
    {
        auto & particles = c->_particles;
        for(auto& i: particles)
        {
            // Calculate velocity
            od::Vec3 velocity = (i->pos - i->lastPos); // * _friction;
            velocity -= velocity * (1.f - _friction) * dt;
            od::Vec3 acc = _gravity;
            
            if(haveBounds())
            {
                if(i->pos.z >= _z2 && velocity.lengthSquared() > 0.00000001)
                {
                    velocity -= velocity * (1.f - _groundFriction) * dt;
                }
            }
            
            // Verlet Integration
            i->lastPos = i->pos;
            i->pos += velocity + acc * dt*dt;
            //i->pos += velocity * dt / lastDt + acc * dt*dt; // time corrected integration
        }
    }
    
    static bool shake = true;
    float iterCoef = 1.f / iter;
    for(auto& c: _composites)
    {
        auto & constraints = c->_constraints;
        for(int i=0; i<iter; i++)
        {
            if(shake)
            {
                for(auto& j: constraints)
                {
                    j->relax(iterCoef);
                }
            }
            else
            {
                for(int j=constraints.size()-1; j>=0; --j)
                {
                    constraints[j]->relax(iterCoef);
                }
            }
            shake ^= 1;
        }
    }
    if(_haveBounds)
    {
        for(auto& c: _composites)
        {
            auto & particles = c->_particles;
            for(auto& i: particles)
            {
                bounds(i);
            }
        }
    }
}

std::weak_ptr<Particle> Verlet::nearestParticle(const od::Vec3 & mouse, int selectionRadius)
{
    std::weak_ptr<Particle> pt;
    float mindist = 0;
    
    for(auto& c: _composites)
    {
        auto & particles = c->_particles;
        for(auto& i: particles)
        {
            float d2 = (i->pos - mouse).lengthSquared();
            if(d2 < selectionRadius*selectionRadius && (pt.expired() || d2 < mindist))
            {
                pt = i;
                mindist = d2;
            }
        }
    }
	return pt;
}

void Verlet::addComposite(std::unique_ptr<Composite> c)
{
    _composites.push_back( std::move(c) );
}

const std::vector< std::unique_ptr<Composite> > & Verlet::composites() const
{
    return _composites;
}
