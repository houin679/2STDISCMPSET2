#include "Player.h"

// Global variable definitions
int n, t1, t2;
int waitingTanks = 0, waitingHealers = 0, waitingDPS = 0, activeInstances = 0;
bool allPlayersQueued = false;
mutex mtx;
condition_variable cv;
vector<Instance> instances;

int main() {
    int numTanks, numHealers, numDPS;

    cout << "Enter n (max instances): ";
    cin >> n;

    cout << "Enter number of Tanks: ";
    cin >> numTanks;

    cout << "Enter number of Healers: ";
    cin >> numHealers;

    cout << "Enter number of DPS: ";
    cin >> numDPS;

    do {
        cout << "Enter t1 (min duration): ";
        cin >> t1;
        cout << "Enter t2 (max duration): ";
        cin >> t2;

        if (t2 < t1) {
            cout << "Error: Maximum duration (t2) cannot be less than minimum duration (t1).\n";
            cout << "Please re-enter the duration values.\n\n";
        }
    } while (t2 < t1);
    
    

    instances.resize(n);

    // Start the manager thread
    thread manager(partyManager);

    // Spawn player threads
    vector<thread> players;
    for (int i = 0; i < numTanks; i++) players.emplace_back(tankThread);
    for (int i = 0; i < numHealers; i++) players.emplace_back(healerThread);
    for (int i = 0; i < numDPS; i++) players.emplace_back(dpsThread);

    // Wait for all players to queue
    for (auto& p : players) p.join();

    {
        lock_guard<mutex> lock(mtx);
        allPlayersQueued = true;
    }
    cv.notify_all();

    manager.join();

    cout << "\n=== SUMMARY ===\n";
    for (int i = 0; i < n; i++) {
        cout << "Instance " << i + 1 << ": "
            << instances[i].partiesServed << " parties, total time "
            << instances[i].totalTime << "s" << endl;
    }

    return 0;
}
