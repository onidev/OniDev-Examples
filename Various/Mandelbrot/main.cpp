#include <onidev.h>
#include <onidev/graphics/texture.h>
#include <cmath>

float mandelbrotSmooth(float x, float y, const int iterations)
{
    float zr=x, zi=y;
    float t;

    int prevIteration = iterations;
    int i=0;
    
    do
    {
        t = 2*zr*zi;
        zr = zr*zr - zi*zi + x;
        zi = t + y;
        i++;
        if( (prevIteration == iterations) && (zr*zr + zi*zi > 4) )
            prevIteration = i+1;
    }
    while(i < prevIteration);
    
    if(i == iterations)
        return 0;
    
    return (i - log2f(log2f(zr*zr + zi*zi)))/iterations;
}

od::Image mandelbrot(const od::ColorRamp & ramp, int wid, int hei, int levels)
{
    od::Image im(wid, hei);
    
    float factor = wid / 3.2f;
    const int n = levels;
    for(int i=0; i<wid; i++)
    for(int j=0; j<hei; j++)
    {
        float sum = 0;
        for(int ii=0; ii<n; ii++)
        for(int jj=0; jj<n; jj++)
        {
            float x = ((i-wid/2)*n + ii);
            float y = ((j-hei/2)*n + jj);
            sum += mandelbrotSmooth(x / (factor * n) - 0.6, y / (factor * n), 30);
        }
        sum /= float(n*n);
        im.setpixel(i, j, ramp.color(sum).toRgba());
    }
    
    return im;
}

int main()
{
    using namespace od;
    Window win(640, 480, "Mandelbrot");
    win.setSynchronization(true);
    
    ColorRamp magma;
    magma.add(0, Color(0, 0, 0));
    magma.add(0.4, Color(194, 28, 18));
    magma.add(0.85, Color(249, 209, 36));
    magma.add(1, Color(252, 255, 214));
    
    Image im( mandelbrot(magma, win.width(), win.height(), 2) );
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
