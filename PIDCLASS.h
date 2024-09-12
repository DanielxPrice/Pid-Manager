#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <bitset>
#define MIN_PID 100
#define MAX_PID 1000
#define AVALABLE_PIDS (MAX_PID - MIN_PID + 1)

using namespace std;

class PidManager
{
public:
    /************************************************************************************
    Constructor
    Description: 
    - calls allocate_map()
    ************************************************************************************/
    PidManager() 
    {
        // creates the map
        if (allocate_map() == -1)
        {
            cerr << "Allocating map failed!";
            exit(1);
        }
    }

    /************************************************************************************
    Function: allocate_map
    Description: This function takes the bitmap from the private section of this class
    and resets it. If it fails that reset, then it throws an exception which will be
    caught and return -1 to indicate that it has failed.
    ************************************************************************************/
    int allocate_map(void)
    {
        try
        {
            pidContainer.reset();
            cout << "Map Allocated!";
            return 1;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Exception caught while making bitmap: " << e.what() << '\n';
            return -1;
        }
        
        
    }
    /************************************************************************************
    Function: allocate_pid
    Description: Loops through the bitmap starting at 0. It sees if there is an avaiable
    slot, and if there is then it will set it to being used. It will make the calculation 
    for the PID and return that number. All ABALABLE_PIDS are taken, then it will return
    -1 showing that it has failed.
    ************************************************************************************/
    int allocate_pid(void)
    {
        for (int i = 0; i < AVALABLE_PIDS; ++i)
        {
            if (pidContainer[i] == 0)
            {
                //Mark that the pid is now being used
                pidContainer.set(i);
                int returnedPid = i + MIN_PID;
                cout << returnedPid << " pid was allocated!";
                return returnedPid;
            }
        }
        //failed, maybe no PIDs avaiable
        return -1;
    }
    /************************************************************************************
    Function: release_pid
    Description: First checks to see if the PID is a valid PID. If it is then it will 
    calculate the pid in association with the pidContainer. Then if it is being used
    "pidContainer[bitmapPid] == 1" then it will reset that index to 0 showing that 
    it was released. If it is not being used it will print to console that it is not
    in use.
    ************************************************************************************/
    void release_pid(int pid)
    {
        if (pid < MIN_PID || pid > MAX_PID)
        {
            cout << "Invalid PID!";
            return;
        }
        int bitmapPid = pid - MIN_PID;
        if (pidContainer[bitmapPid] == 1)
        {
            pidContainer.reset(bitmapPid);
            cout << "\nReleased PID: " << pid << endl;
        }
        else
        {
                cout << "PID " << pid << " not in use, and ready for allocation!\n";
        }
    }


private:
    // Container/data structure that will hold all the PIDs
    bitset<AVALABLE_PIDS> pidContainer;

};