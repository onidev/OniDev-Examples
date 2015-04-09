#include <onidev.h>

int main()
{
    using namespace od;
    Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    Sound sounds[4] = {
        Sound("Sounds/slice.wav"),
        Sound("Sounds/attack.wav"),
        Sound("Sounds/die.wav"),
        Sound("Sounds/hurt.wav")
    };
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        

        drawClear();
        win.updateView();
        
        char key[] = "AZER";
        for(int i=0; i<4; ++i)
        {
            if(keyPressed(key[i]))
                sounds[i].play(true, 1);
        }
        
        win.screenRefresh();
    }
    
    return 0;
}
