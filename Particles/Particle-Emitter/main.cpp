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
    type1->setLife(20, 50);
    type1->colors = ColorGradient( {Color(255, 255, 255), Color(255, 255, 255, 0)} );
    type1->setSprite(ps.sprites(), "dot", 0.f);
    
    auto type2 = std::make_shared<PartType>();
    type2->setLife(20, 50);
    type2->setDirection(0, 360, 0, 0);
    type2->setSpeed(4, 5, -0.05, 0);
    type2->setSize(0.2, 0.2, 0, 0);
    type2->colors = ColorGradient( {Color(255, 255, 255), Color(255, 255, 255, 0)} );
    type2->random_frame = true;
    type2->setSprite(ps.sprites(), "sphere", 0.2f);
    type2->step_number = -4; // faire methode...
    type2->step_type = type1;
    type2->step_emitter = std::make_shared<PartEmitterEllipse>(-16, -16, 16, 16, true);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear(0);
        win.updateView();
        
        if(mouseCheck(mb_left))
        {
            ps.burst(win.mouse_x(), win.mouse_y(), type2, 1);
        }
        
        ps.update(1.f);
        ps.draw();
        
        win.screenRefresh();
    }
    
    return 0;
}
