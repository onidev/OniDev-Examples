#include "constraint.h"
#include <onidev/math/math.h>
#include <cmath>

//https://github.com/subprotocol/verlet-js/tree/master/lib

DistanceConstraint::DistanceConstraint(std::shared_ptr<Particle> a, std::shared_ptr<Particle> b, float stiffness):
    a(a),
    b(b),
    _distanceSquared((a->pos-b->pos).lengthSquared()),
    _stiffness(stiffness)
{
}

void DistanceConstraint::relax(float iterCoef)
{
    od::Vec2 normal = a->pos - b->pos;
    float m = normal.lengthSquared();
    // verifier division 0?
    normal *= (_distanceSquared - m)/m * _stiffness * iterCoef;
    a->pos += normal;
    b->pos -= normal;
}

void DistanceConstraint::draw(const ConstraintRenderer & renderer) const
{
    renderer.draw(*this);
}

void DistanceConstraint::setDistance(float len)
{
    _distanceSquared = len*len;
}

void DistanceConstraint::setStiffness(float stiffness)
{
    _stiffness = stiffness;
}

float DistanceConstraint::distance() const
{
    return sqrtf(_distanceSquared);
}

float DistanceConstraint::distanceSquared() const
{
    return _distanceSquared;
}

float DistanceConstraint::stiffness() const
{
    return _stiffness;
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

PinConstraint::PinConstraint(std::shared_ptr<Particle> a, od::Vec2 pos):
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

void PinConstraint::setPosition(const od::Vec2 & p)
{
    pos = p;
}

od::Vec2 PinConstraint::position() const
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


AngleConstraint::AngleConstraint(std::shared_ptr<Particle> a, std::shared_ptr<Particle> b, std::shared_ptr<Particle> c, float stiffness):
    a(a),b(b),c(c),
    _angle(b->pos.angle(a->pos, c->pos)),
    _stiffness(stiffness)
{
}

void AngleConstraint::relax(float iterCoef)
{
    float angle = b->pos.angle(a->pos, c->pos);
    float diff = angle - this->_angle;
    
    if(diff <= -od::m_pi)
        diff += 2*od::m_pi;
    else if(diff > od::m_pi)
        diff -= 2*od::m_pi;
    
    diff *= iterCoef * _stiffness;
    
    a->pos.rotate(b->pos, diff);
    c->pos.rotate(b->pos,-diff);
    b->pos.rotate(a->pos, diff);
    b->pos.rotate(c->pos,-diff);
    
    //@todo vec2::rotated
}

void AngleConstraint::draw(const ConstraintRenderer & renderer) const
{
    renderer.draw(*this);
}

void AngleConstraint::setAngle(float angle)
{
    _angle = angle;
    if(_angle <= -od::m_pi)
        _angle += 2*od::m_pi;
    else if(_angle > od::m_pi)
        _angle -= 2*od::m_pi;
}

void AngleConstraint::setStiffness(float stiffness)
{
    _stiffness = stiffness;
}

float AngleConstraint::angle() const
{
    return _angle;
}

float AngleConstraint::stiffness() const
{
    return _stiffness;
}

const Particle & AngleConstraint::left() const
{
    return *a;
}

Particle & AngleConstraint::left()
{
    return *a;
}

const Particle & AngleConstraint::middle() const
{
    return *b;
}

Particle & AngleConstraint::middle()
{
    return *b;
}

const Particle & AngleConstraint::right() const
{
    return *c;
}

Particle & AngleConstraint::right()
{
    return *c;
}

