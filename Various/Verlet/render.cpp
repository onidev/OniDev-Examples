#include "render.h"
#include <onidev.h>
#include <cmath>

void MyConstraintRenderer::draw(const DistanceConstraint & c) const
{
    glColor3f(0.8, 0.8, 0.8);
    od::drawLine(c.left().pos.x, c.left().pos.y, c.right().pos.x, c.right().pos.y);
}

void MyConstraintRenderer::draw(const PinConstraint & c) const
{
    
}

void MyConstraintRenderer::draw(const AngleConstraint & c) const
{
    glColor3f(0, 0.8, 0.8);
    
    od::Vec2 a = c.left().pos - c.middle().pos;
    od::Vec2 b = c.right().pos - c.middle().pos;
    
    float r1 = a.lengthSquared();
    float r2 = b.lengthSquared();
    float r = sqrtf(r1<r2?r1:r2) / 2.f;
    
    float a1 = a.angle();
    float a2 = b.angle();
    float wx = c.middle().pos.x;
    float wy = c.middle().pos.y;
    
    if(a1 < a2)
        a1 += od::m_pi*2;
    
    glBegin(GL_LINE_STRIP);
    for(float a=a2; a<=a1; a+=0.1)
    {
        glVertex2f(wx + cosf(a) * r, wy + sinf(a) * r);
    }
    glEnd();
}

void draw(const Verlet & verlet)
{
    MyConstraintRenderer renderer;
    // draw constraints
    for(const auto & composite: verlet.composites())
    {
        for(const auto & c: composite->constraints())
        {
            c->draw(renderer);
        }
    }
    // draw particles
    glColor3f(1, 1, 1);
    for(const auto & composite: verlet.composites())
    {
        for(const auto & pt: composite->particles())
        {
            od::drawCircle(pt->pos.x, pt->pos.y, 2, false);
        }
    }
}
