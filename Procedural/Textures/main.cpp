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
    
    ColorRamp sky;
    sky.add(0, Color(53, 172, 252));
    sky.add(1, Color(215, 233, 252));
    
    ColorRamp magma;
    magma.add(0, Color(0, 0, 0));
    magma.add(0.4, Color(194, 28, 18));
    magma.add(0.85, Color(249, 209, 36));
    magma.add(1, Color(252, 255, 214));
    
    Image im(512, 512);
    
    od::Perlin2 noise;
    
    // Wood
    for(int i=0; i<256; ++i)
    for(int j=0; j<256; ++j)
    {
        float x = i/255.f;
        float y = j/255.f;
        
        float n = 10*noise.noise(x, y);
        n = n - (int)n;
        if(n < 0) n += 1;
        
        n *= 0.9f;
        n += noise.noise(x*200.f, y*200.f) * 0.3f;
        n += noise.noise(x*100.f, y) * 0.5f;
        
        Color c = ramp.color(n);
        im.setpixel(i, j, c.toRgba());
    }
    // Sky
    for(int i=0; i<256; ++i)
    for(int j=0; j<256; ++j)
    {
        float x = i/255.f;
        float y = j/255.f;
        
        float n = noise.fractal(x*3, y*3, 5, 0.6);
        n = (n + 1) / 2.f;
        
        Color c = sky.color(n);
        im.setpixel(i+256, j, c.toRgba());
    }
    // Marble
    for(int i=0; i<256; ++i)
    for(int j=0; j<256; ++j)
    {
        float s = 2;
        float x = s * i / 255.f;
        float y = s * j / 255.f;
        
        float n = noise.fractal(x, y, 5, 0.5);
        n =  sqrtf( fabs( sinf(2.f * m_pi * n) ) );
        
        int g = (n + 1) * 128;
        if(g < 0) g = 0;
        if(g > 255) g = 255;
        im.setpixel(i, j+256, 0xFF000000|g<<16|g<<8|g);
    }
    // misc
    for(int i=0; i<256; ++i)
    for(int j=0; j<256; ++j)
    {
        float s = 1.7;
        float x = s * i / 255.f;
        float y = s * j / 255.f;
        
        
        float n = noise.fractal(x, y, 4, 0.5);
        n =  cosf(20 * 2 * m_pi * (x / 4.f + 0.5f*n));
        
        Color c = magma.color((n + 1) / 2.f);
        im.setpixel(i+256, j+256, c.toRgba());
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
