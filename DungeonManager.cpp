#include "Player.h"
#include <chrono>

// Print the status of all dungeon instances (MUST be called with lock held)
void printInstanceStatus() {
    cout << "\n=== Current Instance Status ===\n";
    for (int i = 0; i < n; ++i) {
        cout << "Instance " << (i + 1) << ": ";
        if (instances[i].active) {
            cout << "active\n";
        }
        else {
            cout << "empty\n";
        }
    }
    cout << "===============================\n";
}

// Simulate running a dungeon instance
void runDungeon(int id, int duration, int tank, int healer, int dps1, int dps2, int dps3) {
    {
        lock_guard<mutex> lock(mtx);
        cout << "\n[Instance " << (id + 1) << "] Party entering dungeon (Duration: " << duration << "s)\n";
        cout << "  Party composition - Tank #" << tank << ", Healer #" << healer
            << ", DPS #" << dps1 << ", #" << dps2 << ", #" << dps3 << "\n";
    }

    // Simulate dungeon run
    this_thread::sleep_for(chrono::seconds(duration));

    {
        lock_guard<mutex> lock(mtx);
        instances[id].active = false;
        instances[id].partiesServed++;
        instances[id].totalTime += duration;
        activeInstances--;

        cout << "\n[Instance " << (id + 1) << "] Party completed dungeon.\n";
        printInstanceStatus();
    }

    cv.notify_all();  // Wake party manager to form more parties
}

// Party manager: forms parties and assigns them to instances
void partyManager() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(t1, t2);

    while (true) {
        unique_lock<mutex> lock(mtx);

        // Wait until there are enough players to form a party AND an available instance
        // OR all players have queued (time to check if we should exit)
        cv.wait(lock, [] {
            return ((waitingTanks >= 1 && waitingHealers >= 1 && waitingDPS >= 3 && activeInstances < n)
                || allPlayersQueued);
            });

        // Exit condition: no more players coming AND can't form more parties AND all instances finished
        if (allPlayersQueued &&
            !(waitingTanks >= 1 && waitingHealers >= 1 && waitingDPS >= 3) &&
            activeInstances == 0) {
            cout << "\n[Party Manager] No more parties can be formed. Shutting down.\n";
            break;
        }

        // Form as many parties as possible while resources allow
        while (waitingTanks >= 1 && waitingHealers >= 1 && waitingDPS >= 3 && activeInstances < n) {
            // Pop players from FIFO queues (ensures fairness, prevents starvation)
            int tank = tankQueue.front(); tankQueue.pop();
            int healer = healerQueue.front(); healerQueue.pop();
            int dps1 = dpsQueue.front(); dpsQueue.pop();
            int dps2 = dpsQueue.front(); dpsQueue.pop();
            int dps3 = dpsQueue.front(); dpsQueue.pop();

            // Update waiting counters
            waitingTanks--;
            waitingHealers--;
            waitingDPS -= 3;

            // Find an available instance
            int instanceId = -1;
            for (int i = 0; i < n; ++i) {
                if (!instances[i].active) {
                    instanceId = i;
                    break;
                }
            }

            if (instanceId == -1) {
                // Safety check: shouldn't happen due to wait condition
                cout << "[Error] No free instance found despite condition check!\n";
                break;
            }

            // Mark instance as active
            instances[instanceId].active = true;
            activeInstances++;

            // Generate random dungeon duration
            int duration = dist(gen);

            cout << "\n[Party Formed] Tank #" << tank << ", Healer #" << healer
                << ", DPS #" << dps1 << ", #" << dps2 << ", #" << dps3 << "\n";
            cout << "Assigned to Instance " << (instanceId + 1) << "\n";

            printInstanceStatus();

            // Launch dungeon in a separate thread
            thread(runDungeon, instanceId, duration, tank, healer, dps1, dps2, dps3).detach();
        }

        // Show current waiting players if any
        if (waitingTanks > 0 || waitingHealers > 0 || waitingDPS > 0) {
            cout << "\nPlayers waiting: " << waitingTanks << " Tanks, "
                << waitingHealers << " Healers, " << waitingDPS << " DPS\n";
        }
    }
}