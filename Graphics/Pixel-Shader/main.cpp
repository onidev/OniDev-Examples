#include <onidev.h>
#include <onidev/graphics/shader.h>

int main()
{
    using namespace od;
    Window win(640, 480, "Pixel Shader Example");
    win.setSynchronization(true);
    
    uint timeUniform;
    float time = 0;
    Shader shader("cosmos.frag", Shader::Fragment);
    
    shader.use();
    shader.uniform(shader.getUniform("resolution"), win.width(), win.height());
    timeUniform = shader.getUniform("time");
    shader.unuse();
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear(0);
        win.updateView();
        
        shader.use();
            time += 0.01f;
            shader.uniform(timeUniform, time);
            drawRectangle(0, 0, win.width(), win.height(), false);
        shader.unuse();
        
        win.screenRefresh();
    }
    
    return 0;
}
