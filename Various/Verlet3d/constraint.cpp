#include "constraint.h"
#include <onidev/math/math.h>
#include <cmath>

DistanceConstraint::DistanceConstraint(std::shared_ptr<Particle> a, std::shared_ptr<Particle> b, float stiffness):
    a(a),b(b),
    m_distanceSquared((a->pos-b->pos).lengthSquared()),
    m_stiffness(stiffness)
{
}

void DistanceConstraint::relax(float iterCoef)
{
    od::Vec3 normal = a->pos - b->pos;
    float m = normal.lengthSquared();
    normal *= (m_distanceSquared - m)/m * m_stiffness * iterCoef;
    a->pos += normal;
    b->pos -= normal;
}

void DistanceConstraint::draw(const ConstraintRenderer & renderer) const
{
    renderer.draw(*this);
}

void DistanceConstraint::setDistance(float len)
{
    m_distanceSquared = len*len;
}

void DistanceConstraint::setStiffness(float stiffness)
{
    m_stiffness = stiffness;
}

float DistanceConstraint::distance() const
{
    return sqrtf(m_distanceSquared);
}

float DistanceConstraint::distanceSquared() const
{
    return m_distanceSquared;
}

float DistanceConstraint::stiffness() const
{
    return m_stiffness;
}

const Particle & DistanceConstraint::left() const
{
    return *a;
}

Particle & DistanceConstraint::left()
{
    return *a;
}

const Particle & DistanceConstraint::right() const
{
    return *b;
}

Particle & DistanceConstraint::right()
{
    return *b;
}


PinConstraint::PinConstraint(std::shared_ptr<Particle> a):
    a(a), pos(a->pos)
{
}

PinConstraint::PinConstraint(std::shared_ptr<Particle> a, od::Vec3 pos):
    a(a), pos(pos)
{
}

void PinConstraint::relax(float iterCoef)
{
    a->pos = pos;
}

void PinConstraint::draw(const ConstraintRenderer & renderer) const
{
    renderer.draw(*this);
}

void PinConstraint::setPosition(const od::Vec3 & p)
{
    pos = p;
}

od::Vec3 PinConstraint::position() const
{
    return pos;
}

const Particle & PinConstraint::particle() const
{
    return *a;
}

Particle & PinConstraint::particle()
{
    return *a;
}
