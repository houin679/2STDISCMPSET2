#include "Player.h"

void tankThread() {
    unique_lock<mutex> lock(mtx);
    waitingTanks++;
    cout << "[Tank] Joined queue. Tanks waiting: " << waitingTanks << endl;
    cv.notify_all();
}
