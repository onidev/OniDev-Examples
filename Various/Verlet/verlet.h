#ifndef VERLET_VERLET_H_INCLUDED
#define VERLET_VERLET_H_INCLUDED

#include "composite.h"
#include <onidev/math/vec2.h>
#include <memory>

class Verlet
{
    od::Vec2 _gravity;
    float _friction;
    float _groundFriction;
    
    float _boundLeft;
    float _boundRight;
    float _boundTop;
    float _boundBottom;
    bool  _haveBounds;
    
    std::vector< std::unique_ptr<Composite> > _composites;
    
    void bounds(std::shared_ptr<Particle> & pt);
    
public:
    Verlet(od::Vec2 gravity, float friction, float groundFriction);
    
    void setBounds(float left, float top, float right, float bottom);
    void disableBounds();
    bool haveBounds() const;
    
    void update(float dt, int iter);
    std::weak_ptr<Particle> nearestParticle(const od::Vec2 & mouse, int selectionRadius = 10);
    
    void addComposite(std::unique_ptr<Composite> c);
    const std::vector< std::unique_ptr<Composite> > & composites() const;
};

#endif // VERLET_VERLET_H_INCLUDED
