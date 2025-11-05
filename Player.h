#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
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

// FIFO queues for fairness (prevent starvation)
extern queue<int> tankQueue;
extern queue<int> healerQueue;
extern queue<int> dpsQueue;

// Thread functions
void tankThread(int id);
void healerThread(int id);
void dpsThread(int id);
void runDungeon(int id, int duration, int tank, int healer, int dps1, int dps2, int dps3);
void partyManager();
void printInstanceStatus();