#include "verlet.h"
#include <cmath>

Verlet::Verlet(od::Vec2 gravity, float friction, float groundFriction):
    _gravity(gravity),
    _friction(friction),
    _groundFriction(groundFriction),
    _haveBounds(false)
{

}

void Verlet::setBounds(float left, float top, float right, float bottom)
{
    _boundLeft = left;
    _boundRight = right;
    _boundTop = top;
    _boundBottom = bottom;
    _haveBounds = true;
}

void Verlet::disableBounds()
{
    _haveBounds = false;
}

bool Verlet::haveBounds() const
{
    return _haveBounds;
}

void Verlet::bounds(std::shared_ptr<Particle> & particle)
{
    if(_haveBounds)
    {
        if(particle->pos.y < _boundTop)
            particle->pos.y = _boundTop;
        if(particle->pos.y > _boundBottom)
            particle->pos.y = _boundBottom;
        if(particle->pos.x < _boundLeft)
            particle->pos.x = _boundLeft;
        if(particle->pos.x > _boundRight)
            particle->pos.x = _boundRight;
    }
    
}

void Verlet::update(float dt, int iter)
{
    //static float lastDt = dt;
    for(auto & c: _composites)
    {
        auto & particles = c->_particles;
        for(auto & i: particles)
        {
            // Calculate velocity
            od::Vec2 velocity = (i->pos - i->lastPos);// * _friction;
            velocity -= velocity * (1.f - _friction) * dt;
            od::Vec2 acc = _gravity;
            
            if(haveBounds())
            {
                if(i->pos.y >= _boundBottom && velocity.lengthSquared() > 0.00000001)
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
    for(auto & c: _composites)
    {
        auto & constraints = c->_constraints;
        for(int i=0; i<iter; i++)
        {
            // l'ordre d'execution influe sur la simulation...
            // trouver une meilleur solution que le 'shake' car pas spécialement efficace...
            if(shake)
            {
                for(auto & j: constraints)
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
    
    for(auto & c: _composites)
    {
        auto & particles = c->_particles;
        for(auto & i: particles)
        {
            bounds(i);
        }
    }
    
    //lastDt = dt;
}

std::weak_ptr<Particle> Verlet::nearestParticle(const od::Vec2 & mouse, int selectionRadius)
{
    std::weak_ptr<Particle> pt;
    float mindist = 0;
    
    for(auto & c: _composites)
    {
        auto & particles = c->_particles;
        for(auto & i: particles)
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
