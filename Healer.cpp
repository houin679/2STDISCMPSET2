#include "Player.h"

void healerThread() {
    {
        lock_guard<mutex> lock(mtx);
        waitingHealers++;
        cout << "[Healer] Joined queue. Healers waiting: " << waitingHealers << endl;
    }
    cv.notify_all();
}
