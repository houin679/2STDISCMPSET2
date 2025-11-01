#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>
using namespace std;

// Struct for each dungeon instance
struct Instance {
    bool active = false;
    int partiesServed = 0;
    int totalTime = 0;
};

// Global shared data
extern int n, t1, t2;
extern int waitingTanks, waitingHealers, waitingDPS, activeInstances;
extern bool allPlayersQueued;
extern mutex mtx;
extern condition_variable cv;
extern vector<Instance> instances;

// Thread functions
void tankThread();
void healerThread();
void dpsThread();
void runDungeon(int id, int duration);
void partyManager();
