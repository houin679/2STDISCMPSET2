#ifndef DUNGEON_MANAGER_H
#define DUNGEON_MANAGER_H

#include "Player.h"

// Manages the formation and coordination of parties
void partyManager();

// Runs a dungeon instance with a specific duration and assigned players
void runDungeon(int instanceID, int duration, int tank, int healer, int dps1, int dps2, int dps3);

// Prints current status of all dungeon instances
void printInstanceStatus();

#endif
