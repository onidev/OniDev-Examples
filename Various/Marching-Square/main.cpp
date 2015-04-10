#include <onidev.h>
#include <onidev/noise/perlin.h>

struct Point
{
    float x, y, v;
    Point():x(0),y(0),v(0){}
    Point(float x, float y, float v):x(x),y(y),v(v){}
};

Point interpolation(const Point & p1, const Point & p2, float v)
{
    return Point(p1.x + (p2.x - p1.x)*v, p1.y + (p2.y - p1.y)*v, 0);
}

void cas2(Point & p1, Point & p2, Point & p3, Point & p4, float v)
{
    Point pa, pb;
    
    float v1 = (v - p4.v) / (p1.v - p4.v);
    float v2 = (v - p4.v) / (p3.v - p4.v);
    
    pa = interpolation (p4, p1, v1);
    pb = interpolation (p4, p3, v2);
    
    glBegin(GL_LINES);
    glVertex3f (pa.x, pa.y, v1);
    glVertex3f (pb.x, pb.y, v2);
    glEnd();
}

void cas3(Point & p1, Point & p2, Point & p3, Point & p4, float v)
{
    Point pa, pb;
    
    float v1 = (v - p1.v) / (p2.v - p1.v);
    float v2 = (v - p3.v) / (p4.v - p3.v);
    
    pa = interpolation (p1, p2, v1);
    pb = interpolation (p3, p4, v2);
    
    glBegin (GL_LINES);
    glVertex3f (pa.x, pa.y, v1);
    glVertex3f (pb.x, pb.y, v2);
    glEnd ();
}

void cas4(Point & p1, Point & p2, Point & p3, Point & p4, float v)
{
    Point pa, pb, pc, pd;
    
    float v1 = (v - p1.v) / (p2.v - p1.v);
    float v2 = (v - p2.v) / (p3.v - p2.v);
    float v3 = (v - p3.v) / (p4.v - p3.v);
    float v4 = (v - p4.v) / (p1.v - p4.v);
    
    pa = interpolation (p1, p2, v1);
    pb = interpolation (p2, p3, v2);
    pc = interpolation (p3, p4, v3);
    pd = interpolation (p4, p1, v4);
    
    glBegin (GL_LINES);
    glVertex3f (pa.x, pa.y, v1);
    glVertex3f (pb.x, pb.y, v2);
    glVertex3f (pc.x, pc.y, v3);
    glVertex3f (pd.x, pd.y, v4);
    glEnd ();
}

int main()
{
    using namespace od;
    Window win(512, 512, "OniDev app");
    win.setSynchronization(true);
    
    Image im(512, 512);
    Perlin2 noise;
    for(int j=0; j<im.height(); ++j)
    for(int i=0; i<im.width(); ++i)
    {
        float x = i/(float)im.width();
        float y = j/(float)im.height();
        
        float n = noise.fractal(x, y, 8, 0.6);
        n = (n + 1) / 2.f;
        
        Color c(n*255, n*255, n*255);
        im.setpixel(i, j, c.toRgba());
    }
    
    
    Texture tex(im);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear(0xFF000000);
        win.updateView();
        
        if(!keyCheck(vk_space))
            tex.draw(0, 0);
        
        int scale = 8;
        glLoadIdentity();
        glScalef(scale, scale, 1);
        
        int levels = 32;
        
        for(int n=0; n<levels; ++n)
        {
        float level = (float)n/levels;
        glColor4f(1 - level/2.f, level/2.f, level, 0.5);
        
        for(int i=0; i<im.width()-scale; i+=scale)
        for(int j=0; j<im.height()-scale; j+=scale)
        {
            Point p1(i/scale  , j/scale  , (im.getpixel(i      , j      ) & 0xFF) / 255.f);
            Point p2(i/scale+1, j/scale  , (im.getpixel(i+scale, j      ) & 0xFF) / 255.f);
            Point p3(i/scale+1, j/scale+1, (im.getpixel(i+scale, j+scale) & 0xFF) / 255.f);
            Point p4(i/scale  , j/scale+1, (im.getpixel(i      , j+scale) & 0xFF) / 255.f);
            int bits = (p1.v < level)<<3 | (p2.v < level)<<2 | (p3.v < level)<<1 | (p4.v < level);
            
            // Resolve saddle point
            if(!keyCheck(vk_enter))
            if(bits == 5 || bits == 10)
            {
                // Method 1
                //float median = (p1.v + p2.v + p3.v + p4.v) / 4.f;
                // Method 2
                //float median = (im.getpixel(i+scale/2, j+scale/2) & 0xFF) / 255.f;
                
                // Method 1 + 2
                float median1 = (p1.v + p2.v + p3.v + p4.v) / 4.f;
                float median2 = (im.getpixel(i+scale/2, j+scale/2) & 0xFF) / 255.f;
                float median = (median1 + median2) / 2.f;
                if(median < level)
                {
                    if(bits == 5)
                        bits = 10;
                    else
                        bits = 5;
                }
            }
            
            switch(bits)
            {
                case  0: break;
                case  1: cas2(p1, p2, p3, p4, level); break;
                case  2: cas2(p4, p1, p2, p3, level); break;
                case  3: cas3(p2, p3, p4, p1, level); break;
                case  4: cas2(p3, p4, p1, p2, level); break;
                case  5: cas4(p1, p2, p3, p4, level); break;
                case  6: cas3(p1, p2, p3, p4, level); break;
                case  7: cas2(p2, p3, p4, p1, level); break;
                case  8: cas2(p2, p3, p4, p1, level); break;
                case  9: cas3(p1, p2, p3, p4, level); break;
                case 10: cas4(p4, p1, p2, p3, level); break;
                case 11: cas2(p3, p4, p1, p2, level); break;
                case 12: cas3(p2, p3, p4, p1, level); break;
                case 13: cas2(p4, p1, p2, p3, level); break;
                case 14: cas2(p1, p2, p3, p4, level); break;
                case 15: break;
            }
        }
        }
        glColor3f(1, 1, 1);
        
        win.screenRefresh();
    }
    
    return 0;
}

