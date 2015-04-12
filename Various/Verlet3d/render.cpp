#include "render.h"
#include <onidev.h>

void MyConstraintRenderer::draw(const DistanceConstraint & c) const
{
    if(m_renderShadows)
    {
        glVertex3f(c.left().pos.x, c.left().pos.y, 0.1);
        glVertex3f(c.right().pos.x, c.right().pos.y, 0.1);
    }
    else
    {
        glVertex3f(c.left().pos.x, c.left().pos.y, c.left().pos.z);
        glVertex3f(c.right().pos.x, c.right().pos.y, c.right().pos.z);
    }
}

void draw(const Verlet & verlet)
{
    MyConstraintRenderer renderer;
    // draw constraints
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_LINES);
    for(const auto& composite: verlet.composites())
    {
        for(const auto& c: composite->constraints())
        {
            c->draw(renderer);
        }
    }
    glEnd();
    
    // draw particles
    glColor3f(1, 1, 1);
    glEnable( GL_POINT_SMOOTH );
    glPointSize(5.f);
    glBegin(GL_POINTS);
    for(const auto& composite: verlet.composites())
    {
        for(const auto& pt: composite->particles())
        {
            glVertex3f(pt->pos.x, pt->pos.y, pt->pos.z);
        }
    }
    glEnd();
    
    // Draw shadows
    renderer.renderShadows();
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_LINES);
    for(const auto& composite: verlet.composites())
    {
        for(const auto& c: composite->constraints())
        {
            c->draw(renderer);
        }
    }
    glEnd();
    
    glPointSize(4.f);
    glBegin(GL_POINTS);
    for(const auto& composite: verlet.composites())
    {
        for(const auto& pt: composite->particles())
        {
            glVertex3f(pt->pos.x, pt->pos.y, 0.1);
        }
    }
    glEnd();
}
