#include "Player.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>
using namespace std;

// === Shared Globals (declared extern in Player.h) ===
extern int n, t1, t2;
extern int waitingTanks, waitingHealers, waitingDPS;
extern int activeInstances;
extern bool allPlayersQueued;

extern mutex mtx;
extern condition_variable cv;

extern vector<Instance> instances;

// === Function Prototypes ===
void runDungeon(int id, int duration);
void printInstanceStatus();

// === Function Definitions ===

// Print the status of all dungeon instances
void printInstanceStatus() {
    // NO lock_guard here!
    cout << "\n=== Current Instance Status ===\n";
    for (int i = 0; i < n; ++i) {
        cout << "Instance " << i + 1 << ": "
            << (instances[i].active ? "active" : "empty") << "\n";
    }
    cout << "===============================\n";
}

// Simulate running a dungeon instance
void runDungeon(int id, int duration) {
    {
        lock_guard<mutex> lock(mtx);
        cout << "[Instance " << id + 1 << "] Running for " << duration << "s" << endl;
    }

    this_thread::sleep_for(chrono::seconds(duration));

    {
        lock_guard<mutex> lock(mtx);
        instances[id].active = false;
        instances[id].partiesServed++;
        instances[id].totalTime += duration;
        activeInstances--;
        cout << "[Instance " << id + 1 << "] Finished." << endl;
        printInstanceStatus(); // Show updated status after dungeon finishes
    }

    cv.notify_all(); // wake the manager to form more parties
}

// Party manager: forms parties and assigns them to instances
void partyManager() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(t1, t2);

    while (true) {
        unique_lock<mutex> lock(mtx);

        // Wait until there are enough players OR all players are queued
        cv.wait(lock, [] {
            return ((waitingTanks >= 1 && waitingHealers >= 1 && waitingDPS >= 3 && activeInstances < n)
                || allPlayersQueued);
            });

        // Exit condition — nothing left to do
        if (allPlayersQueued &&
            !(waitingTanks >= 1 && waitingHealers >= 1 && waitingDPS >= 3) &&
            activeInstances == 0) {
            break;
        }

        // Form as many parties as possible while resources allow
        while (waitingTanks >= 1 && waitingHealers >= 1 && waitingDPS >= 3 && activeInstances < n) {
            waitingTanks--;
            waitingHealers--;
            waitingDPS -= 3;

            // Find an available instance
            int id = -1;
            for (int i = 0; i < n; ++i) {
                if (!instances[i].active) {
                    id = i;
                    break;
                }
            }

            if (id == -1) {
                // no free instance — shouldn’t happen due to condition, but safety first
                break;
            }

            // Mark instance active before releasing lock
            instances[id].active = true;
            activeInstances++;

            int duration = dist(gen);

            cout << "[Party Formed] Assigned to Instance " << id + 1
                << " (Duration: " << duration << "s)" << endl;
            printInstanceStatus(); // Show current instance occupancy

            // Launch dungeon thread
            thread(runDungeon, id, duration).detach();
        }
    }
}
