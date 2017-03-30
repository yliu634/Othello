#include <iostream>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "othello.h"
#include "muloth.h"
#include <chrono>
#include <inttypes.h>
#include <algorithm>
#include "io_helper.h"
#include "time.h"
#include <cmath>
using namespace std;
typedef unsigned long long keyT;
typedef uint16_t valueT;

keyT *keys;
valueT *values;

map<keyT,valueT> mapp;

bool inline check(Othello<keyT> & oth , keyT key, valueT value) {
             
    valueT q = (valueT) oth.queryInt(key);
    if (q != value) {
         printf("%llx: %lx!=%lx \n", key, q, value);
         return false;
    }
    return true;

}
void printHisto(map<valueT, int> &vv) {
    double s = 0.0, s2 = 0.0;
    for (auto &a:vv) {
        s += a.second;
        s2 += ((double) a.second)*(a.second);
        //printf("%d:%d\t", a.first, a.second);
    }
    double avg = s*1.0/vv.size();
    printf("avg = %.1lf, stdE = %.1lf, cnt = %d\n", avg, sqrt(s2/vv.size() - avg*avg), vv.size()); 

}
void computeRates(Othello<keyT> &oth) {
    map<valueT,int> vv;
    int NNN = 104857600;
    for (int i = 0; i < NNN; i++) {
        keyT key = (((unsigned long long) rand())<<32) + rand();
        valueT ans =  oth.queryInt(key);
//        valueT ans = rand();
        vv[ans] ++;
    }
    printHisto(vv);
}
int main(int argc, char * argv[]) {
    keys = (keyT*) valloc(sizeof(keyT) * 4096);
    values = (valueT*) valloc(sizeof(valueT)*4096);
    int N = 1124;
    for (int i = 0 ; i < 4096; i++) {
        keys[i] = (((unsigned long long) rand())<<32) + rand();
        values[i] = (rand()&0xFFFF);
    }
    Othello<keyT> oth(16,&keys[0],N, false, &values[0], sizeof(values[0]),0);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
    printf("Add 100 keys\n");
    oth.addkeys(100, &(values[0]), sizeof(values[0]));
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 


    printf("Remove one key currently at location 0 \n");
    values[0] = values[oth.mykeycount-1];
    oth.removeOneKey(0);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 


    printf("Remove one key currently at location 5 \n");
    values[5] = values[oth.mykeycount-1];
    oth.removeOneKey(5);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 


    for (int i = oth.mykeycount ; i < oth.mykeycount+200; i++) {
        keys[i] = (((unsigned long long) rand())<<32) + rand();
        values[i] = rand();
    }

    printf("Add 200 keys\n");
    oth.addkeys(200, &(values[0]), sizeof(values[0]));
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
 
    printf("Modify 10 random values\n");
    for (int t = 0; t < 10; t++) {
        int i = rand() % oth.mykeycount;
        values[i] = (rand()&0xFFFF);
        oth.updatevalue(i,&(values[0]),sizeof(values[0]));
        if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
    }

    oth.setAlienPreference(&(values[0]), sizeof(values[0]), 0.5);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
    computeRates(oth);

    oth.setAlienPreference(&(values[0]), sizeof(values[0]), 0.8);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
    computeRates(oth);
    for (double pref = 0.1; pref < 0.5; pref+=0.049) {
    oth.setAlienPreference(&(values[0]), sizeof(values[0]), pref);
    printf("Othello size = %d\n", oth.mykeycount);    for (int i = 0 ; i < oth.mykeycount; i++) if (!check(oth, keys[i], values[i])) printf("Err at %d\n",i); 
    computeRates(oth);
    }

    free(keys);
    free(values);
    return 0;
    
}
