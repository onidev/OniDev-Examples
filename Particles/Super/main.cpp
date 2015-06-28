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
    
    auto ice = std::make_shared<PartType>();
    ice->setLife(40, 40);
    ice->colors = ColorGradient( {Color(255, 255, 255), Color(255, 255, 255, 0)} );
    ice->setSprite(ps.sprites(), "ice", 0.1f);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear(0);
        win.updateView();
        
        if(mousePressed(mb_left))
        {
            for(int i=0; i<360; i+=360/8)
            {
                PartSuper super;
                super.vx = lendirx(2, i);
                super.vy = lendiry(2, i);
                super.angle = (i - 135) * 3.1415 / 180.f;
                ps.burst(win.mouse_x() + lendirx(8, i), win.mouse_y() + lendiry(8, i), super, ice, 1);
            }
        }
        
        ps.update(1.f);
        ps.draw();
        
        win.screenRefresh();
    }
    
    return 0;
}
