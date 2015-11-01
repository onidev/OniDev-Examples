// Copyright 2013-2015 OniDev, All Rights Reserved.

/*=============================================================================
	Cell based gamedev tutorial
    #2 Flood fill algorithm
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
    typedef int Position;
    
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
    inline const T& at(Position p) const {
        return _cells[p];
    }
    inline T& at(Position p) {
        return _cells[p];
    }
    
    inline constexpr Position pos(int i, int j) const {
        return i + _wid * j;
    }
    
    Position pickUp(float x, float y) const
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
    
    // requires bool isValid(T)
    // requires void validate(T)
    template<typename IsValid, typename Validate>
    void floodFill(int i, int j, IsValid isValid, Validate validate)
    {
        // naive stack based flood fill algorithm
        std::stack< std::pair<int, int> > stack;
        stack.push( {i, j} );
        while(!stack.empty())
        {
            auto top = stack.top();
            int x = top.first;
            int y = top.second;
            stack.pop();
            
            auto& cell = at(x, y);
            if(isValid(cell) || x < 0 || y < 0 || x >= _wid || y >= _hei)
                continue;
            
            validate(cell);
            
            stack.push( {x-1, y} );
            stack.push( {x+1, y} );
            stack.push( {x, y-1} );
            stack.push( {x, y+1} );
        }
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
    Position _mouse_pos;
    
public:
    CellMap(unsigned wid, unsigned hei, float wx, float wy);
    void step(float dt);
    void draw() const;
};

CellMap::CellMap(unsigned wid, unsigned hei, float wx, float wy):
    Grid<Cell>(wid, hei, wx, wy)
{
    for(unsigned i=0; i<_hei; ++i)
    {
        at(_wid/2, i).value = 2;
    }
    for(unsigned i=0; i<_wid; ++i)
    {
        at(i, _hei/2).value = 2;
    }
}

void CellMap::step(float dt)
{
    const od::View& view = od::View::current();
    _mouse_pos = pickUp(view.mouse().x, view.mouse().y);
    
    if(od::mousePressed(od::mb_left) && _mouse_pos != -1)
    {
        int fill = 2;
        if(_cells[_mouse_pos].value == 2)
            fill = 0;
        
        floodFill(_mouse_pos%_wid, _mouse_pos/_wid,
            [&fill](Cell& c) { return c.value == fill; },
            [&fill](Cell& c) { c.value = fill; }
        );
    }
    
    if(od::mousePressed(od::mb_right) && _mouse_pos != -1)
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
    Window win(32*12, 32*12, "Flood fill tutorial");
    win.setSynchronization(true);
    
    assetsLoadSprites("../res/assets.xml");
    
    CellMap map(11, 11, 16, 16);
    
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
