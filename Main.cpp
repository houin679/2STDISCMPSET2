#include "DungeonManager.h"

// Global definitions
int n, t1, t2;
int waitingTanks = 0;
int waitingHealers = 0;
int waitingDPS = 0;
int activeInstances = 0;

mutex mtx;
condition_variable cv;

vector<Instance> instances;

int main() {
    int numTanks, numHealers, numDPS;
    cout << "Enter n (max instances): ";
    cin >> n;
    cout << "Enter tanks, healers, DPS: ";
    cin >> numTanks >> numHealers >> numDPS;
    cout << "Enter t1 (min) and t2 (max) duration: ";
    cin >> t1 >> t2;

    instances.resize(n);

    // Start the party manager
    thread manager(partyManager);

    // Spawn players
    vector<thread> players;
    for (int i = 0; i < numTanks; i++) players.emplace_back(tankThread);
    for (int i = 0; i < numHealers; i++) players.emplace_back(healerThread);
    for (int i = 0; i < numDPS; i++) players.emplace_back(dpsThread);

    for (auto& p : players) p.join();

    manager.detach();

    // Wait until all instances finish
    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        bool anyActive = false;
        {
            lock_guard<mutex> lock(mtx);
            for (auto& inst : instances) {
                if (inst.active) anyActive = true;
            }
        }
        if (!anyActive) break;
    }

    cout << "\n=== SUMMARY ===\n";
    for (int i = 0; i < n; i++) {
        cout << "Instance " << i + 1 << ": "
            << instances[i].partiesServed << " parties, total time "
            << instances[i].totalTime << "s\n";
    }

    return 0;
}
