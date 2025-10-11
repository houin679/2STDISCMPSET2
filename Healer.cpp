#include "Player.h"

void healerThread() {
    unique_lock<mutex> lock(mtx);
    waitingHealers++;
    cout << "[Healer] Joined queue. Healers waiting: " << waitingHealers << endl;
    cv.notify_all();
}
