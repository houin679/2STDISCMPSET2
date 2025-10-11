#include "Player.h"

void dpsThread() {
    unique_lock<mutex> lock(mtx);
    waitingDPS++;
    cout << "[DPS] Joined queue. DPS waiting: " << waitingDPS << endl;
    cv.notify_all();
}
