#include "Player.h"

void dpsThread(int id) {
    {
        lock_guard<mutex> lock(mtx);
        dpsQueue.push(id);
        waitingDPS++;
        cout << "[DPS #" << id << "] Joined queue. Total DPS waiting: " << waitingDPS << endl;
    }
    cv.notify_all();  // Notify party manager that a player has queued
}