#ifndef AUTOMATA_H_INCLUDED
#define AUTOMATA_H_INCLUDED

#include <vector>

class CellularAutomata
{
    std::vector<unsigned char> map, old;
    int wid, hei;

public:
    CellularAutomata(int wid, int hei);
    void draw(int x, int y, int scale);
    void update();
    void set(int x, int y, int v);
    void clear();
};

#endif // AUTOMATA_H_INCLUDED
