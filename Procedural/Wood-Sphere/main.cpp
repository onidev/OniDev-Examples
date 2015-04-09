#include <onidev.h>
#include <onidev/noise/perlin.h>
#include <cmath>

int main()
{
    using namespace od;
    Window win(512, 512, "OniDev app");
    win.setSynchronization(true);
    
    ColorRamp ramp;
    ramp.add(0, Color(98, 52, 22));
    ramp.add(1, Color(156, 105, 44));
    
    Image im(512, 512);
    
    od::Perlin2 noise2;
    od::Perlin3 noise;
    
    // la fonction multifractal n'est pas équivalente a l'ancienne fractalNoise2d
    /// @todo regler les soucis causés par ce changement...
    
    int wid = 512;
    int hei = 512;
    float r = 220;
    for(int i=0; i<wid; ++i)
    for(int j=0; j<hei; ++j)
    {
        float x = (i - wid/2) / r;
        float y = (j - hei/2) / r;
        
        float z = 1 - x*x - y*y;
        Color c(0, 0, 0);
        if(z >= 0)
        {
            z = sqrtf(z);
            
            float x = i/(float)wid;
            float y = j/(float)hei;
            
            float n = 10*noise.noise(x, y, z);
            n = n - (int)n;
            if(n < 0) n += 1;
            
            n *= 0.9f;
            n += noise.noise(x*200.f, y*200.f, z) * 0.3f;
            n += noise.noise(x*100.f, y, z) * 0.5f;
            
            c = ramp.color(n);
            float shadow = (1 + x - y + z) / 3.f;
            c.r *= shadow;
            c.g *= shadow;
            c.b *= shadow;
        }
        im.setpixel(i, j, c.toRgba());
    }
    
    Texture tex(im);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        tex.draw(0, 0);
        win.screenRefresh();
    }
    
    return 0;
}
