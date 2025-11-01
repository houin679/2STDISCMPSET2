#include "Player.h"

void tankThread() {
    {
        lock_guard<mutex> lock(mtx);
        waitingTanks++;
        cout << "[Tank] Joined queue. Tanks waiting: " << waitingTanks << endl;
    }
    cv.notify_all();
}
