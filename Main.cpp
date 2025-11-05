#include "Player.h"

// Global variable definitions
int n, t1, t2;
int waitingTanks = 0, waitingHealers = 0, waitingDPS = 0, activeInstances = 0;
bool allPlayersQueued = false;
mutex mtx;
condition_variable cv;
vector<Instance> instances;

// FIFO queues for fair player selection
queue<int> tankQueue;
queue<int> healerQueue;
queue<int> dpsQueue;

int main() {
    int numTanks, numHealers, numDPS;

    cout << "=== LFG Dungeon Queue Simulator ===\n\n";

    cout << "Enter n (max concurrent instances): ";
    cin >> n;
    cout << "Enter number of Tanks: ";
    cin >> numTanks;
    cout << "Enter number of Healers: ";
    cin >> numHealers;
    cout << "Enter number of DPS: ";
    cin >> numDPS;

    do {
        cout << "Enter t1 (min dungeon duration in seconds): ";
        cin >> t1;
        cout << "Enter t2 (max dungeon duration in seconds): ";
        cin >> t2;
        if (t2 < t1) {
            cout << "Error: Maximum duration (t2) cannot be less than minimum duration (t1).\n";
            cout << "Please re-enter the duration values.\n\n";
        }
    } while (t2 < t1);

    instances.resize(n);

    cout << "\n=== Starting Queue System ===\n\n";

    // Start the party manager thread
    thread manager(partyManager);

    // Spawn player threads with unique IDs
    vector<thread> players;
    for (int i = 0; i < numTanks; i++) {
        players.emplace_back(tankThread, i + 1);
    }
    for (int i = 0; i < numHealers; i++) {
        players.emplace_back(healerThread, i + 1);
    }
    for (int i = 0; i < numDPS; i++) {
        players.emplace_back(dpsThread, i + 1);
    }

    // Wait for all players to queue up
    for (auto& p : players) {
        p.join();
    }

    // Signal that all players have queued
    {
        lock_guard<mutex> lock(mtx);
        allPlayersQueued = true;
        cout << "\n[System] All players have queued.\n";
    }
    cv.notify_all();

    // Wait for party manager to finish
    manager.join();

    // Final summary
    cout << "\n=================================\n";
    cout << "=== FINAL SUMMARY ===\n";
    cout << "=================================\n";
    for (int i = 0; i < n; i++) {
        cout << "Instance " << (i + 1) << ": "
            << instances[i].partiesServed << " parties served, "
            << "Total time: " << instances[i].totalTime << " seconds\n";
    }

    // Show leftover players
    if (waitingTanks > 0 || waitingHealers > 0 || waitingDPS > 0) {
        cout << "\n=== LEFTOVER PLAYERS (Could not form complete party) ===\n";
        cout << "Tanks: " << waitingTanks << "\n";
        cout << "Healers: " << waitingHealers << "\n";
        cout << "DPS: " << waitingDPS << "\n";
    }
    else {
        cout << "\n[System] All players were matched into parties!\n";
    }

    cout << "=================================\n";

    return 0;
}