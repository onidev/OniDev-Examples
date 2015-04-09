#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "verlet.h"

class MyConstraintRenderer : public ConstraintRenderer
{
    void draw(const DistanceConstraint & c) const;
    void draw(const PinConstraint & c) const;
    void draw(const AngleConstraint & c) const;
};

void draw(const Verlet & verlet);

#endif // RENDER_H_INCLUDED
