#include <onidev.h>

int main()
{
    using namespace od;
    Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    PartSystem ps;
    
    TexturePacker packer("Particles", true, false);
    packer.allowCropAlpha(false);
    packer.setMargin(1);
    packer.build();
    packer.buildMeta(ps.sprites());
    
    auto type1 = std::make_shared<PartType>();
    type1->setLife(100, 100);
    type1->setSpeed(2, 3, -0.05, 0);
    type1->setDirection(0, 360, 0, 0);
    type1->colors.add(Color(255, 255, 0));
    type1->colors.add(Color(255, 0, 0));
    type1->colors.add(Color(255, 0, 0, 0));
    type1->setSize(0.3, 0.5, 0, 0);
    type1->setSprite(ps.sprites(), "flare", 0.f);
    
    auto type2 = std::make_shared<PartType>();
    type2->setLife(100, 150);
    type2->setSpeed(5, 8, 0, 0);
    type2->setGravity(0.2, 270);
    type2->setDirection(90-20, 90+20, 0, 0);
    type2->colors.add(Color(255, 255, 255));
    type2->colors.add(Color(0, 0, 255, 0));
    type2->setSize(0.3, 0.5, 0, 0);
    type2->setSprite(ps.sprites(), "sphere", 0.f);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear(0);
        win.updateView();
        
        if(mouseCheck(mb_left))
        {
            ps.burst(win.mouse_x(), win.mouse_y(), type1, 3);
        }
        if(mouseCheck(mb_right))
        {
            ps.burst(win.mouse_x(), win.mouse_y(), type2, 3);
        }
        
        ps.update(1.f);
        ps.draw();
        
        win.screenRefresh();
    }
    
    return 0;
}
