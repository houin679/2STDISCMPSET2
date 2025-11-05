#include "Player.h"

void tankThread(int id) {
    {
        lock_guard<mutex> lock(mtx);
        tankQueue.push(id);
        waitingTanks++;
        cout << "[Tank #" << id << "] Joined queue. Total tanks waiting: " << waitingTanks << endl;
    }
    cv.notify_all();  // Notify party manager that a player has queued
}