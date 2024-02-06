#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <ctime>
typedef struct
{
    int size;
    int ** mat;
} Matrix;
typedef struct
{
    int x;
    int y;
} Point;

typedef struct{
    int reward;
    int * Point;
} Sequence;

#endif