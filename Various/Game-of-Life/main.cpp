#include <onidev.h>
#include "automata.h"

int main()
{
    using namespace od;
    Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    int scale = 8;
    CellularAutomata automata(win.width()/scale, win.height()/scale);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        
        automata.draw(0, 0, scale);

        if(mouseCheck(mb_left))
        {
            automata.set(win.mouse_x()/scale, win.mouse_y()/scale, 1);
        }

        if(keyPressed('C'))
            automata.clear();

        if(keyCheck(vk_space) || keyCheck(vk_enter))
        {
            automata.update();
        }
        
        win.screenRefresh();
    }
    
    return 0;
}
