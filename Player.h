#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <random>
using namespace std;

// Shared globals
extern int n;               // max concurrent instances
extern int t1, t2;          // dungeon time range
extern int waitingTanks;
extern int waitingHealers;
extern int waitingDPS;
extern int activeInstances;

extern mutex mtx;
extern condition_variable cv;

struct Instance {
    bool active = false;
    int partiesServed = 0;
    int totalTime = 0;
};

extern vector<Instance> instances;

// Player thread functions
void tankThread();
void healerThread();
void dpsThread();

#endif
