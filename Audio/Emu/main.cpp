#include <onidev.h>
#include "audioEmu.h"

int main()
{
    using namespace od;
    Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    AudioEmu gen("zelda_oos.gbs", 1, 44100);
    SoundStream music(gen);
    music.play();
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        
        if(keyPressed('S'))
        {
            music.stop();
        }
        else if(keyPressed('P'))
        {
            if(music.status() == Music::Paused || music.status() == Music::Stopped)
                music.play();
            else
                music.pause();
        }
        
        if(music.status() != Music::Stopped)
        {
            if(keyPressed(vk_left) && gen.track() > 0)
            {
                music.stop();
                gen.changeTrack(gen.track() - 1);
                music.play();
            }
            else
            if(keyPressed(vk_right))
            {
                music.stop();
                gen.changeTrack(gen.track() + 1);
                music.play();
            }
        }
        
        win.screenRefresh();
    }
    
    return 0;
}
