#include <stdlib.h>
#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <math.h>

#define ll long long
#define ull unsigned long long

typedef char* string;

struct Para{
    int s, S;
    int E;
    int b, B;
    int hits, misses, evicts, verbose;
};

struct Line{
    int uses, valid;
    ull tag;
    string block;
};

struct Set{
    struct Line* lines;
};

struct Cache{
    struct Set* sets;
};

struct Cache cacheBuild(){
    
}



int main()
{
    printSummary(0, 0, 0);
    return 0;
}
