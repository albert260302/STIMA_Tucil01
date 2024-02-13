#ifndef MAIN_H
#define MAIN_H

#include "boolean.h"
#include <string>

typedef struct 
{
    char a;
    char b;
} Token;

typedef struct
{
    int col_length;
    int row_length;
    Token ** mat;
} Matrix;
typedef struct
{
    int x;
    int y;
} Point;

typedef struct{
    int reward;
    int Neff;
    Point * point;
} Buffer;

typedef struct{
    int reward;
    Token * seq;
    int Neff;
    boolean taken;
}Sequence;

typedef struct{
    int Neff;
    Sequence * arr;
} ListSeq;

typedef struct
{
    int Neff;
    Token * token;
} ListToken;

#endif