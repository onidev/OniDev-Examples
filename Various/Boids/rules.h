#ifndef RULE_H_INCLUDED
#define RULE_H_INCLUDED

#include "particle.h"

class BoidRule
{
public:
    virtual od::Vec2 getVelocity(Particle & pt, std::vector<Particle> & boids) = 0;
};

class Barycenter : public BoidRule
{
    float min_distance_squared;
    float max_distance_squared;
    float coeff;
    
public:
    Barycenter(float min_distance, float max_distance, float coeff);
    od::Vec2 getVelocity(Particle & pt, std::vector<Particle> & boids);
};

class Repulse : public BoidRule
{
    float min_distance_squared;
    float coeff;
    
public:
    Repulse(float min_distance, float coeff);
    od::Vec2 getVelocity(Particle & pt, std::vector<Particle> & boids);
};

class RepulseAbsolute : public BoidRule
{
    float min_distance_squared;
    float coeff;
    
public:
    RepulseAbsolute(float min_distance, float coeff);
    od::Vec2 getVelocity(Particle & pt, std::vector<Particle> & boids);
};

class MouseRepule : public BoidRule
{
    float min_distance_squared;
    float coeff;
    
public:
    MouseRepule(float min_distance, float coeff);
    od::Vec2 getVelocity(Particle & pt, std::vector<Particle> & boids);
};

class AverageDirection : public BoidRule
{
    float min_distance_squared;
    float coeff;
    
public:
    AverageDirection(float min_distance, float coeff);
    od::Vec2 getVelocity(Particle & pt, std::vector<Particle> & boids);
};

class PushedByEdges : public BoidRule
{
    od::Vec2 size;
    float margin;
    
public:
    PushedByEdges(od::Vec2 size, float margin);
    od::Vec2 getVelocity(Particle & pt, std::vector<Particle> & boids);
};

#endif // RULE_H_INCLUDED
