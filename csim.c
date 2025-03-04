/* 23b1num1587 Dalaitseren B*/

#include <stdlib.h>
#include "cachelab.h"
#include <limits.h>
#include <getopt.h>
#include <stdio.h>
#include <math.h>

#define ll long long
#define ull unsigned long long

typedef char* string;
int LRU = 0;
struct Para{
    int s, S;   // Sets
    int E;      // Lines
    int b, B;   // Blocks
    int hits, misses, evicts, verbose;
};

struct Line{
    int lru, valid;
    ull tag;
    string block;
};

struct Set{
    struct Line* lines;
};

struct Cache{
    struct Set* sets;
};

void printPara(struct Para *p){
    printf("\n===> Parameters.\ns: %d\n", p -> s);
    printf("S: %d\n", p -> S);
    printf("E: %d\n", p -> E);
    printf("b: %d\n", p -> b);
    printf("B: %d\n", p -> B);
}

void printCache(struct Cache cache, struct Para* param){
    for(int i =0 ; i < param -> S; i++){
        for(int j =0 ; j < param -> E; j++){
            printf("|%d|%llx|%s|%d   ", cache.sets[i].lines[j].valid, cache.sets[i].lines[j].tag, cache.sets[i].lines[j].block, cache.sets[i].lines[j].lru);
        }
        printf("\n");
    }
}
void access(ull mem_address, struct Para *param, struct Cache cache){
    // printf("==> Accessing...");
    ull tag = mem_address >> (param -> s + param -> b);
    ull set_index = (mem_address >> param -> b) & (ull)(param -> S - 1);
    struct Line* lines = cache.sets[set_index].lines;

    // printf("%llx %llx\n", mem_address, tag);

    for(int i = 0; i < param -> E; i++){
        if(lines[i].valid){
            if(lines[i].tag == tag){
                // printf("Hit\n");
                lines[i].lru = LRU++;
                param -> hits++;
                return;
            }
        }
    }

    // printf("Miss\n");

    param -> misses++;

    ull eviction_lru = ULONG_MAX;
    ull eviction_line = 0;


    for(int i = 0; i < param -> E; i++){
        if(eviction_lru > lines[i].lru){
            eviction_line = i;
            eviction_lru = lines[i].lru;
        }
    }

    if(lines[eviction_line].valid){
        param -> evicts++;
    }

    lines[eviction_line].valid = 1;
    lines[eviction_line].tag = tag;
    lines[eviction_line].lru = LRU++;
    
}


struct Cache cacheBuild(struct Para* param){
    // printf("==> Building...");
    struct Cache cache;
    cache.sets = (struct Set *)malloc(sizeof(struct Set) * (param -> S));
    
    for(int i = 0; i < param -> S; i++){
        cache.sets[i].lines = (struct Line*)malloc(sizeof(struct Line) * (param -> E));
        for(int j = 0; j < param -> E; j++){
            cache.sets[i].lines[j].lru = 0;
            cache.sets[i].lines[j].valid = 0;
            cache.sets[i].lines[j].tag = 0;
        }
    }
    
    return cache;
}

void simulate(string trace_filename, struct Para* param, struct Cache cache){
    // printf("==> Simulating...");
    FILE* trace_file = fopen(trace_filename, "r");
    char cmd;
    ull address;
    int size;

    while(fscanf(trace_file, " %c %llx,%d", &cmd, &address, &size) == 3){
        // printf("%llx\n", address);
        switch(cmd){
            case 'L':
                access(address, param, cache); 
                break;
            case 'S':
                access(address, param, cache); 
                break;
            case 'M':
                access(address, param, cache); 
                access(address, param, cache);
                break;
            default: 
                break;
        }
        // printCache(cache, param);
    }



    fclose(trace_file);
}

void freeCache(struct Para* param, struct Cache cache) {
    for (int i = 0; i < param -> S; i++) {
        free(cache.sets[i].lines);
    }
    free(cache.sets);
}

void usage(string arg[]) {
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", arg[0]);
    printf("Options:\n");
    printf(" -h        Print this help message.\n");
    printf(" -v        Optional verbose flag.\n");
    printf(" -s <num>  Number of set index bits.\n");
    printf(" -E <num>  Number of lines per set.\n");
    printf(" -b <num>  Number of block offset bits.\n");
    printf(" -t <file> Trace file.\n");
    printf("\nExamples:\n");
    printf(" linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", arg[0]);
    printf(" linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", arg[0]);
    exit(0);
}

int main(int argc, string argv[])
{
    struct Para *param = malloc(sizeof *param);
    string trace_filename;

    for(int i = 0; i < argc; i++){
        // printf("%s\n", argv[i]);
        if(argv[i][0] == '-'){
            switch (argv[i][1]){
                case 's':
                    param -> s = atoi(argv[++i]);
                    param -> S = (1 << param -> s);
                    break;
                case 'E':
                    param -> E = atoi(argv[++i]);
                    break;
                case 'b':
                    param -> b = atoi(argv[++i]);
                    param -> B = (1 << param -> b);
                    break;
                case 't':
                    trace_filename = argv[++i];
                    break;
                case 'v':
                    param -> verbose = 1;
                    break;
                case 'h':
                    usage(argv);
                    exit(0);
                default:
                    usage(argv);
                    exit(1);
            }
        }
    }

    // printPara(param);
    // printf("%s", trace_filename);

    struct Cache main_cache = cacheBuild(param);
    simulate(trace_filename, param, main_cache);

    printSummary(param -> hits, param -> misses, param -> evicts);
    return 0;
}
