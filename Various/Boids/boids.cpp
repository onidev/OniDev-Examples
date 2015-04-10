#include "boids.h"
#include <onidev.h>

Boids::Boids(od::Vec2 worldSize):
    _resolution(worldSize),
    _surf(worldSize.x, worldSize.y),
    _tex_tail("tail.png"),
    _tex_head("head.png")
{
}

void Boids::add(const Particle & pt)
{
    _particles.push_back(pt);
}

void Boids::add(float x, float y, int color)
{
    _particles.push_back( Particle(x, y, color) );
}

void Boids::simulate()
{
    od::Vec2 mouse(od::View::mouse_x(), od::View::mouse_y());
    for(auto & b: _particles)
    {
        for(auto & rule: _rules)
        {
            b.vel += rule->getVelocity(b, _particles);
        }
        
        b.vel.normalize();
        b.vel *= 1.5f;
        b.pos += b.vel;
        
        b.loopWorld(_resolution);
    }
}

void Boids::updateRenderBuffer()
{
    _surf.setTarget();
    
    glColor4f(0, 0, 0, 0.1f);
    od::drawRectangle(0, 0, _resolution.x, _resolution.y);
    glColor3f(1, 1, 1);
    
    glBlendFunc(GL_ONE, GL_ONE);
    _tex_tail.bind();
    for(auto & p: _particles)
    {
        od::Color c(p.color);
        glColor3ub(c[0], c[1], c[2]);
        _tex_tail.draw(p.pos.x - _tex_tail.width()/2.f, p.pos.y - _tex_tail.height()/2.f);
    }
    _tex_tail.unbind();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    _surf.resetTarget();
}

void Boids::draw() const
{
    glColor3f(1, 1, 1);
    _surf.draw(0, 0);
    
    glBlendFunc(GL_ONE, GL_ONE);
    _tex_head.bind();
    for(auto & p: _particles)
    {
        od::Color c(p.color);
        glColor3ub(c[0], c[1], c[2]);
        _tex_head.draw(p.pos.x - _tex_head.width()/2.f, p.pos.y - _tex_head.height()/2.f);
    }
    _tex_head.unbind();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
