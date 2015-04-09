#include "automata.h"
#include <onidev/graphics.h>

CellularAutomata::CellularAutomata(int wid, int hei):
    map(wid * hei),
    old(wid * hei),
    wid(wid), hei(hei)
{
}

void CellularAutomata::draw(int wx, int wy, int scale)
{
    for(int j=0; j<hei; ++j)
    {
        for(int i=0; i<wid; ++i)
        {
            if(map[i + j*wid] > 0)
            {
                int x = wx + i*scale;
                int y = wy + j*scale;
                od::drawRectangle(x, y, x+scale, y+scale, false);
            }
        }
    }
}

void CellularAutomata::update()
{
    for(int j=0; j<hei; ++j)
    {
        for(int i=0; i<wid; ++i)
        {
            int p = i + j*wid;
            old[p] = map[p];
        }
    }
    
    for(int j=1; j<hei-1; ++j)
    {
        for(int i=1; i<wid-1; ++i)
        {
            int nei = 0;
            for(int x=-1; x<=1; ++x)
            {
                for(int y=-1; y<=1; ++y)
                {
                    if(x == 0 && y == 0)
                        continue;

                    if(old[i+x + (j+y)*wid])
                        nei++;
                }
            }
            
            int p = i + j*wid;
            if(old[p] == 0)
            {
                if(nei == 3)
                    map[p] = 1;
            }
            else
            {
                if(nei != 2 && nei != 3)
                    map[p] = 0;
            }
        }
    }
}

void CellularAutomata::set(int x, int y, int v)
{
    map[x + y*wid] = v;
}

void CellularAutomata::clear()
{
    std::fill(map.begin(), map.end(), 0);
}


