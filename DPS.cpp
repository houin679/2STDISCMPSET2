#include "Player.h"

void dpsThread() {
    {
        lock_guard<mutex> lock(mtx);
        waitingDPS++;
        cout << "[DPS] Joined queue. DPS waiting: " << waitingDPS << endl;
    }
    cv.notify_all();
}
