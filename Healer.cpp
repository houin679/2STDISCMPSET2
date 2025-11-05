#include "Player.h"

void healerThread(int id) {
    {
        lock_guard<mutex> lock(mtx);
        healerQueue.push(id);
        waitingHealers++;
        cout << "[Healer #" << id << "] Joined queue. Total healers waiting: " << waitingHealers << endl;
    }
    cv.notify_all();  // Notify party manager that a player has queued
}