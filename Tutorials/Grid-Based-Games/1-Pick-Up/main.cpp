// Copyright 2013-2015 OniDev, All Rights Reserved.

/*=============================================================================
	Cell based gamedev tutorial
    #1 Grid pick up
=============================================================================*/

#include <onidev.h>

/*-----------------------------------------------------------------------------
	Generic grid template class
-----------------------------------------------------------------------------*/

template<typename T>
class Grid
{
protected:
    unsigned _wid;
    unsigned _hei;
    std::vector<T> _cells;
    
    // Offset
    float _wx;
    float _wy;
    
public:
    Grid(unsigned wid, unsigned hei, float wx, float wy):
        _wid(wid),
        _hei(hei),
        _cells(wid * hei),
        _wx(wx),
        _wy(wy)
    {
    }
    
    inline const T& at(int i, int j) const {
        return _cells[i + j*_wid];
    }
    inline T& at(int i, int j) {
        return _cells[i + j*_wid];
    }
    inline const T& at(int p) const {
        return _cells[p];
    }
    inline T& at(int p) {
        return _cells[p];
    }
    
    inline constexpr int pos(int i, int j) const {
        return i + _wid * j;
    }
    
    int pickUp(float x, float y) const
    {
        int px = x - _wx;
        int py = y - _wy;
        if(px >= 0 && py >= 0)
        {
            px /= T::Size;
            py /= T::Size;
            if(px < _wid && py < _hei)
            {
                return px + py * _wid;
            }
        }
        return -1;
    }
};

/*-----------------------------------------------------------------------------
	Custom grid implementation
-----------------------------------------------------------------------------*/

struct Cell
{
    static const int Size = 32;
    int value;
    
    Cell(): value(0) {}
};

class CellMap : public Grid<Cell>
{
    int _mouse_pos;
    
public:
    CellMap(unsigned wid, unsigned hei, float wx, float wy);
    void step(float dt);
    void draw() const;
};

CellMap::CellMap(unsigned wid, unsigned hei, float wx, float wy):
    Grid<Cell>(wid, hei, wx, wy)
{
}

void CellMap::step(float dt)
{
    const od::View& view = od::View::current();
    _mouse_pos = pickUp(view.mouse().x, view.mouse().y);
    
    if(od::mousePressed(od::mb_left) && _mouse_pos != -1)
    {
        auto& cell = at(_mouse_pos);
        if(cell.value == 0)
            cell.value = 2;
        else
            cell.value = 0;
    }
}

void CellMap::draw() const
{
    for(unsigned j=0; j<_hei; ++j)
    for(unsigned i=0; i<_wid; ++i)
    {
        unsigned p = pos(i, j);
        int subimage = 1;
        if(p != _mouse_pos)
            subimage = at(p).value;
        
        od::drawSprite("tile", subimage, _wx + i * Cell::Size, _wy + j * Cell::Size);
    }
}

/*-----------------------------------------------------------------------------
	Main
-----------------------------------------------------------------------------*/

int main()
{
    using namespace od;
    Window win(352, 352, "Pick up tutorial");
    win.setSynchronization(true);
    
    assetsLoadSprites("../res/assets.xml");
    
    CellMap map(10, 10, 16, 16);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        
        map.step(1.f);
        map.draw();
        
        win.screenRefresh();
    }
    
    return 0;
}
