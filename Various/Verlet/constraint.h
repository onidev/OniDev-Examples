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
    float _distanceSquared;
    float _stiffness;

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
    od::Vec2 pos;
    
public:
    PinConstraint(std::shared_ptr<Particle> a);
    PinConstraint(std::shared_ptr<Particle> a, od::Vec2 pos);
    
    void relax(float iterCoef);
    void draw(const ConstraintRenderer & renderer) const;
    
    void setPosition(const od::Vec2 & pos);
    od::Vec2 position() const;
    
    const Particle & particle() const;
    Particle & particle();
};


class AngleConstraint : public Constraint
{
    std::shared_ptr<Particle> a;
    std::shared_ptr<Particle> b;
    std::shared_ptr<Particle> c;
    
    float _angle;
    float _stiffness;
    
public:
    AngleConstraint(std::shared_ptr<Particle> a, std::shared_ptr<Particle> b, std::shared_ptr<Particle> c, float stiffness);
    
    void relax(float iterCoef);
    void draw(const ConstraintRenderer & renderer) const;
    
    void setAngle(float angle);
    void setStiffness(float stiffness);
    
    float angle() const;
    float stiffness() const;
    
    const Particle & left() const;
    Particle & left();
    
    const Particle & middle() const;
    Particle & middle();
    
    const Particle & right() const;
    Particle & right();
};


struct ConstraintRenderer
{
    virtual void draw(const DistanceConstraint & c) const = 0;
    virtual void draw(const PinConstraint & c) const = 0;
    virtual void draw(const AngleConstraint & c) const = 0;
};

#endif // VERLET_CONSTRAINT_H_INCLUDED
