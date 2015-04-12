#include "composite.h"
#include <algorithm>

std::shared_ptr<Particle> Composite::addParticle(const od::Vec3 & pos)
{
    return addParticle(pos, od::Vec3(0, 0));
}

std::shared_ptr<Particle> Composite::addParticle(const od::Vec3 & pos, const od::Vec3 & vel)
{
    std::shared_ptr<Particle> pt(new Particle(pos, vel));
    _particles.push_back(pt);
    return pt;
}

bool Composite::removeParticle(std::shared_ptr<Particle> & pt)
{
    auto it = std::find(_particles.begin(), _particles.end(), pt);
    if(it != _particles.end())
    {
        _particles.erase(it);
        return true;
    }
    return false;
}

bool Composite::removeConstraint(std::shared_ptr<Constraint> & c)
{
    auto it = std::find(_constraints.begin(), _constraints.end(), c);
    if(it != _constraints.end())
    {
        _constraints.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Particle> Composite::getParticleAt(unsigned int ind)
{
    if(ind < _particles.size())
        return _particles[ind];
    return nullptr;
}
