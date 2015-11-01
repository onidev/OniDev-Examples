// Copyright 2013-2015 OniDev, All Rights Reserved.

/*=============================================================================
	Cell based gamedev tutorial
    #3 Smooth Movements
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
    int jewel;
    float dx;
    float dy;
    
    Cell(): value(0),jewel(0),dx(0),dy(0) {}
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
    at(_wid/2, _hei/2).jewel = 1;
}

void CellMap::step(float dt)
{
    const od::View& view = od::View::current();
    _mouse_pos = pickUp(view.mouse().x, view.mouse().y);
    
    // start horizontal movement
    if(od::mousePressed(od::mb_left) && _mouse_pos != -1)
    {
        auto& cell = _cells[_mouse_pos];
        if(cell.jewel != 0 && cell.dx == 0 && cell.dy == 0)
        {
            if(_mouse_pos%_wid == 0)
                cell.dx = dt;
            else if(_mouse_pos%_wid == _wid-1)
                cell.dx =-dt;
            else
                cell.dx = (rand()%2)? -dt:dt;
        }
    }
    // start vertical movement
    if(od::mousePressed(od::mb_right) && _mouse_pos != -1)
    {
        auto& cell = _cells[_mouse_pos];
        if(cell.jewel != 0 && cell.dx == 0 && cell.dy == 0)
        {
            if(_mouse_pos/_wid == 0)
                cell.dy = dt;
            else if(_mouse_pos/_wid == _hei-1)
                cell.dy =-dt;
            else
                cell.dy = (rand()%2)? -dt:dt;
        }
    }
    
    // move jewels
    for(unsigned j=0; j<_hei; ++j)
    for(unsigned i=0; i<_wid; ++i)
    {
        auto& cell = _cells[i + j * _wid];
        if(cell.jewel != 0)
        {
            if(cell.dx > 0)
            {
                cell.dx += dt;
                if(cell.dx >= Cell::Size)
                {
                    cell.dx = 0;
                    std::swap(cell.jewel, _cells[i+1 + j * _wid].jewel);
                }
            }
            else if(cell.dx < 0)
            {
                cell.dx -= dt;
                if(cell.dx <= -Cell::Size)
                {
                    cell.dx = 0;
                    std::swap(cell.jewel, _cells[i-1 + j * _wid].jewel);
                }
            }
            if(cell.dy > 0)
            {
                cell.dy += dt;
                if(cell.dy >= Cell::Size)
                {
                    cell.dy = 0;
                    std::swap(cell.jewel, _cells[i + (j+1) * _wid].jewel);
                }
            }
            else if(cell.dy < 0)
            {
                cell.dy -= dt;
                if(cell.dy <= -Cell::Size)
                {
                    cell.dy = 0;
                    std::swap(cell.jewel, _cells[i + (j-1) * _wid].jewel);
                }
            }
        }
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
    
    for(unsigned j=0; j<_hei; ++j)
    for(unsigned i=0; i<_wid; ++i)
    {
        auto& cell = at(i, j);
        if(cell.jewel != 0)
            od::drawSprite("jewels", cell.jewel-1, _wx + i * Cell::Size + cell.dx, _wy + j * Cell::Size + cell.dy);
    }
}

/*-----------------------------------------------------------------------------
	Main
-----------------------------------------------------------------------------*/

int main()
{
    using namespace od;
    Window win(32*12, 32*12, "Smooth movements tutorial");
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
