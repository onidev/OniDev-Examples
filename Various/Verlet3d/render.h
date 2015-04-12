#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "verlet.h"

class MyConstraintRenderer : public ConstraintRenderer
{
    bool m_renderShadows;
    
    void draw(const DistanceConstraint & c) const;
    void draw(const PinConstraint & c) const {}
    
public:
    MyConstraintRenderer():m_renderShadows(false) {}
    void renderShadows() { m_renderShadows = true; }
};

void draw(const Verlet & verlet);

#endif // RENDER_H_INCLUDED
