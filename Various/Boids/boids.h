#ifndef BOIDS_H_INCLUDED
#define BOIDS_H_INCLUDED

#include <onidev/graphics/surface.h>
#include "rules.h"
#include <memory>

class Boids
{
    od::Vec2 _resolution;
    od::Surface _surf;
    od::Texture _tex_tail;
    od::Texture _tex_head;
    
    std::vector<Particle> _particles;
    std::vector< std::unique_ptr<BoidRule> > _rules;
    
public:
    Boids(od::Vec2 worldSize);
    
    template <class C, class... Args> void addRule(Args&&... args)
    {
        std::unique_ptr<BoidRule> rule(new C(std::forward<Args>(args)...));
        _rules.push_back( std::move(rule) );
    }
    
    void add(const Particle & pt);
    void add(float x, float y, int color = 0);
    void simulate();
    
    void updateRenderBuffer();
    void draw() const;
};

#endif // BOIDS_H_INCLUDED
