#  Dungeon Queue Synchronization (PSet2)

This program simulates the **LFG (Looking for Group)** dungeon queuing system of an MMORPG — designed to demonstrate **process synchronization** using C++ threads, mutexes, and condition variables.

---

##  Problem Overview

In this simulation:

- Only **n dungeon instances** can be active concurrently.
- A standard party is composed of:
  -  1 Tank
  -  1 Healer
  -  3 DPS
- Each dungeon run has a **randomized duration** between `t1` (fastest) and `t2` (slowest) seconds.
- Players queue simultaneously, and the system must:
  - Avoid **deadlocks**
  - Prevent **starvation**
  - Ensure **fair party formation** using FIFO queues

---

##  Features

- Threaded simulation of Tanks, Healers, and DPS players.
- Dynamic party formation managed by a central `partyManager()`.
- Reusable dungeon instances with tracking of:
  - Active/empty status
  - Number of parties served
  - Total time served
- Randomized dungeon clear times (`t1`–`t2`) to simulate player skill variability.
- Proper synchronization via `std::mutex` and `std::condition_variable`.

---


###  How to Run/Build/Compilation Step

1. Open **Visual Studio 2022**.  
2. Go to **File → Open → Project/Solution** and select your `.sln` file.  
3. Ensure all source files (`main.cpp`, `DungeonManager.cpp`, `Player.h`, `DungeonManager.h`) are part of the project.  
4. Set **Configuration** to `Debug` or `Release` (either works).  
5. Click **▶ Run** or **Start Debugging (F5)** to build and run the program.

