#ifndef VERLET_CONSTRAINT_H_INCLUDED
#define VERLET_CONSTRAINT_H_INCLUDED

#include "particle.h"
#include <memory>

struct ConstraintRenderer;

struct Constraint
{
    virtual ~Constraint() {}
    virtual void relax(float iterCoef) = 0;
    virtual void draw(const ConstraintRenderer & renderer) const = 0;
};


class DistanceConstraint : public Constraint
{
    std::shared_ptr<Particle> a;
    std::shared_ptr<Particle> b;
    float m_distanceSquared;
    float m_stiffness;

public:
    DistanceConstraint(std::shared_ptr<Particle> a, std::shared_ptr<Particle> b, float stiffness);
    
    void relax(float iterCoef);
    void draw(const ConstraintRenderer & renderer) const;
    
    void setDistance(float len);
    void setStiffness(float stiffness);
    
    // add distanceSquared
    float distanceSquared() const;
    float distance() const;
    float stiffness() const;
    
    const Particle & left() const;
    Particle & left();
    
    const Particle & right() const;
    Particle & right();
};


class PinConstraint : public Constraint
{
    std::shared_ptr<Particle> a;
    od::Vec3 pos;
    
public:
    PinConstraint(std::shared_ptr<Particle> a);
    PinConstraint(std::shared_ptr<Particle> a, od::Vec3 pos);
    
    void relax(float iterCoef);
    void draw(const ConstraintRenderer & renderer) const;
    
    void setPosition(const od::Vec3 & pos);
    od::Vec3 position() const;
    
    const Particle & particle() const;
    Particle & particle();
};

struct ConstraintRenderer
{
    virtual void draw(const DistanceConstraint & c) const = 0;
    virtual void draw(const PinConstraint & c) const = 0;
};

#endif // VERLET_CONSTRAINT_H_INCLUDED
