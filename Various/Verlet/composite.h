#ifndef VERLET_COMPOSITE_H_INCLUDED
#define VERLET_COMPOSITE_H_INCLUDED

#include "particle.h"
#include "constraint.h"

#include <vector>
#include <memory>

// Composite is a factory
// You don't have to preocup to memoty
class Composite final
{
    friend class Verlet;
    std::vector< std::shared_ptr<Particle> >   _particles;
    std::vector< std::shared_ptr<Constraint> > _constraints;
    
public:
    // Factory - Insert methods
    std::shared_ptr<Particle> addParticle(const od::Vec2 & pos);
    std::shared_ptr<Particle> addParticle(const od::Vec2 & pos, const od::Vec2 & vel);
    
    template <class MyConstraint, class... Args> std::shared_ptr<MyConstraint> addConstraint(Args&&... args)
    {
        std::shared_ptr<MyConstraint> c(new MyConstraint(std::forward<Args>(args)...));
        _constraints.push_back(c);
        return c;
    }
    
    // Remove methods
    bool removeParticle(std::shared_ptr<Particle> & pt);
    bool removeConstraint(std::shared_ptr<Constraint> & c);
    
    // Get method
    std::shared_ptr<Particle> getParticleAt(unsigned int ind);
    
    const std::vector< std::shared_ptr<Particle> > & particles() const { return _particles; }
    const std::vector< std::shared_ptr<Constraint> > & constraints() const { return _constraints; }
};

#endif
