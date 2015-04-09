#include <onidev.h>
#include "generator.h"

int main()
{
    using namespace od;
    Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    Generator gen(44100);
    SoundStream music(gen);
    music.play();
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        
        win.screenRefresh();
    }
    
    return 0;
}
