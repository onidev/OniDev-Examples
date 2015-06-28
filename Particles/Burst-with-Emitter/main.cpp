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
    type1->setLife(50, 100);
    type1->colors = ColorGradient( {Color(255, 255, 255), Color(255, 255, 255, 0)} );
    type1->random_frame = true;
    type1->setSprite(ps.sprites(), "digits", 0.2f);
    
    auto em1 = std::make_shared<PartEmitterEllipse>(-80, -80, 80, 80, false);
    auto em2 = std::make_shared<PartEmitterRectangle>(-80, -80, 80, 80, false);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear(0);
        win.updateView();
        
        if(mouseCheck(mb_left))
        {
            ps.burst(win.mouse_x(), win.mouse_y(), *em1, type1, 3);
        }
        if(mouseCheck(mb_right))
        {
            ps.burst(win.mouse_x(), win.mouse_y(), *em2, type1, 3);
        }
        
        ps.update(1.f);
        ps.draw();
        
        win.screenRefresh();
    }
    
    return 0;
}
