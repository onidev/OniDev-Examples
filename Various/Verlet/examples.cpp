#include <onidev.h>
#include "make_unique.h"
#include "verlet.h"

void createSolidBlob(Verlet & verlet, float wx, float wy, float r)
{
    using namespace od;
    auto blob = std::make_unique<Composite>();
    
    for(int i=0; i<360; i+=30)
    {
        float x = wx + lendirx(r, i);
        float y = wy + lendiry(r, i);
        blob->addParticle(Vec2(x, y));
    }
    for(size_t i=0; i<blob->particles().size(); ++i)
    {
        for(size_t j=0; j<blob->particles().size(); ++j)
        {
            if(i > j)
                blob->addConstraint<DistanceConstraint>(blob->getParticleAt(i), blob->getParticleAt(j), 1);
        }
    }
    verlet.addComposite( std::move(blob) );
}

void createTree(Verlet & verlet, float wx, float wy)
{
    using namespace od;
    auto tree = std::make_unique<Composite>();
    
    auto root = tree->addParticle(Vec2(wx, wy));
    auto base = tree->addParticle(Vec2(wx, wy - 12));
    tree->addConstraint<PinConstraint>(root);
    tree->addConstraint<PinConstraint>(base);
    
    std::function< std::shared_ptr<Particle>(std::shared_ptr<Particle>, float, Vec2, int, int)> addBranchRecursively =
        [&tree, &addBranchRecursively](std::shared_ptr<Particle> parent, float coeff, Vec2 normal, int iter, int iterMax) -> std::shared_ptr<Particle>
    {
        auto pt = tree->addParticle(parent->pos + normal * 80 * coeff);
        tree->addConstraint<DistanceConstraint>(pt, parent, 1);
        
        if(iter != 0)
        {
            auto left = addBranchRecursively(pt, coeff*coeff, normal.rotated(-35 * m_pi/180.f), iter-1, iterMax);
            auto right= addBranchRecursively(pt, coeff*coeff, normal.rotated( 35 * m_pi/180.f), iter-1, iterMax);
            float stiff = lerp(0.7, 0, 1.f - (float)iter/iterMax);
            tree->addConstraint<AngleConstraint>(left, pt, parent, stiff);
            tree->addConstraint<AngleConstraint>(parent, pt, right, stiff);
        }
        return pt;
    };
    
    tree->addConstraint<AngleConstraint>(root, base, addBranchRecursively(base, 0.9, Vec2(0, -1), 4, 4), 1);
    
    verlet.addComposite( std::move(tree) );
}

void createRope(Verlet & verlet, float wx, float wy, int count, float sep)
{
    using namespace od;
    auto rope = std::make_unique<Composite>();
    {
        auto base = rope->addParticle(Vec2(wx, wy));
        rope->addConstraint<PinConstraint>(base);
        for(int i=0; i<count; ++i)
        {
            wy += sep;
            auto pt = rope->addParticle(Vec2(wx, wy));
            rope->addConstraint<DistanceConstraint>(base, pt, 1);
            base = pt;
        }
    }
    verlet.addComposite( std::move(rope) );
}

void createCloth(Verlet & verlet, float wx, float wy, int count_h, int count_v, float sep)
{
    using namespace od;
    auto cloth = std::make_unique<Composite>();
    
    std::vector< std::shared_ptr<Particle> > line;
    for(int i=0; i<count_h; ++i)
    {
        auto pt = cloth->addParticle(Vec2(wx + i*sep, wy));
        cloth->addConstraint<PinConstraint>(pt);
        line.push_back(pt);
    }
    
    for(int j=0; j<count_v; j++)
    {
        std::vector< std::shared_ptr<Particle> > lineTemp;
        wy += sep;
        std::shared_ptr<Particle> prev;
        for(int i=0; i<count_h; ++i)
        {
            auto pt = cloth->addParticle(Vec2(wx + i*sep, wy));
            lineTemp.push_back(pt);
            cloth->addConstraint<DistanceConstraint>(pt, line[i], 1);
            if(prev)
                cloth->addConstraint<DistanceConstraint>(pt, prev, 1);
            prev = pt;
        }
        line = lineTemp;
    }
    verlet.addComposite( std::move(cloth) );
}
