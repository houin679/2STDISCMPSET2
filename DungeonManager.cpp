#include "DungeonManager.h"

void runDungeon(int id, int duration) {
    {
        lock_guard<mutex> lock(mtx);
        instances[id].active = true;
        activeInstances++;
    }

    cout << "[Instance " << id + 1 << "] Active for " << duration << "s\n";
    this_thread::sleep_for(chrono::seconds(duration));

    {
        lock_guard<mutex> lock(mtx);
        instances[id].active = false;
        instances[id].partiesServed++;
        instances[id].totalTime += duration;
        activeInstances--;
    }

    cout << "[Instance " << id + 1 << "] Finished.\n";
    cv.notify_all();
}

void partyManager() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(t1, t2);

    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] {
            return (waitingTanks >= 1 && waitingHealers >= 1 && waitingDPS >= 3 && activeInstances < n);
            });

        // Form a party
        waitingTanks--;
        waitingHealers--;
        waitingDPS -= 3;

        int id = -1;
        for (int i = 0; i < n; i++) {
            if (!instances[i].active) {
                id = i;
                break;
            }
        }
        if (id == -1) continue;

        int duration = dist(gen);
        thread(runDungeon, id, duration).detach();
    }
}
