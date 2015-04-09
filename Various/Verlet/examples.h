#ifndef EXAMPLES_H_INCLUDED
#define EXAMPLES_H_INCLUDED

#include "verlet.h"

void createSolidBlob(Verlet & verlet, float wx, float wy, float r);
void createTree(Verlet & verlet, float wx, float wy);
void createRope(Verlet & verlet, float wx, float wy, int count, float sep);
void createCloth(Verlet & verlet, float wx, float wy, int count_h, int count_v, float sep);


#endif // EXAMPLES_H_INCLUDED
