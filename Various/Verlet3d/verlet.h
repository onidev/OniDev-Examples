#ifndef VERLET_VERLET_H_INCLUDED
#define VERLET_VERLET_H_INCLUDED

#include "composite.h"
#include <onidev/math/vec2.h>
#include <memory>

class Verlet
{
    od::Vec3 _gravity;
    float _friction;
    float _groundFriction;
    bool _haveBounds;
    float _x1, _y1, _z1;
    float _x2, _y2, _z2;
    
    std::vector< std::unique_ptr<Composite> > _composites;
    
    void bounds(std::shared_ptr<Particle> & pt);
    
public:
    Verlet(od::Vec3 gravity, float friction, float groundFriction):
        _gravity(gravity),_friction(friction),_groundFriction(groundFriction),_haveBounds(false){}
    
    void setBounds(float x1, float y1, float z1, float x2, float y2, float z2);
    void disableBounds() { _haveBounds = false; }
    bool haveBounds() const { return _haveBounds; }
    
    void update(float dt, int iter);
    std::weak_ptr<Particle> nearestParticle(const od::Vec3 & mouse, int selectionRadius = 10);
    
    void addComposite(std::unique_ptr<Composite> c);
    const std::vector< std::unique_ptr<Composite> > & composites() const;
};

#endif // VERLET_VERLET_H_INCLUDED
