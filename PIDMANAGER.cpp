#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>

#define MIN_PID 100
#define MAX_PID 1000
#include "PIDCLASS.h"

using namespace std;

int main() 
{
    vector<int> parentPids;
    vector<int> childPids;

    pid_t pid;
    pid = fork();

    if (pid < 0)
    {
        cout << "Failed Fork";
        return 1;
    }
    //CHILD PROCESS
    else if (pid == 0)
    {
        PidManager childPidManager;
        //childPidManager.allocate_map();
        cout << "\nAllocating and Releasing one by one";
        cout << "\nChild Pids:\n";
        // Now have it do stuff like allocate some pids and release them
        for (int i = 0; i < 3; ++i)
        {
            int otherPid = childPidManager.allocate_pid();
            if (otherPid != -1)
            {
                childPids.push_back(otherPid);
                sleep(1);
                childPidManager.release_pid(otherPid);

                // chck to see if the released PID becomes available for allocation again
                childPidManager.release_pid(otherPid);
            }
        }

        //Testing Area
        // cout << "childPids Vector";
        // for (int element : childPids)
        // {
        //     cout << element << " ";
        // }
    }
    //PARENT PROCESS
    else 
    {
        wait(NULL);
        PidManager parentPidManager;
        //parentPidManager.allocate_map(); //already called in construction
        cout << "\n\nParent Pids:\n";

        //Allocate Pids
        for (int i = 0; i < 3; ++i)
        {
            int ppid = parentPidManager.allocate_pid();
            cout << "Parent pid " << (i + 1) << ": " << ppid << endl;
            parentPids.push_back(ppid);
        }

        // Release and Test to see if release Pids become available again
        cout << "Releaseing Pids";
        for (int i = 0; i < 2; ++i)
        {
            if (i == 1)
            {
                cout << "\nCheck if release Pids are available again:\n";
            }
            for (int element : parentPids)
            {
                parentPidManager.release_pid(element);
            }
        }

        cout << "\nCheck to see if you can allocate same pid that was released\n";
        int ppid = parentPidManager.allocate_pid();
        auto it = find(parentPids.begin(), parentPids.end(), ppid);
        if (it != parentPids.end())
        {
            cout << "\nPreviously allocated pid that was release was allocated again!";
        }
        else
        {
            cout << "error";
        }
        parentPidManager.release_pid(ppid);
        
        

        //Testing Area
        // cout << "\nparentPids Vector:\n";
        // for (int element : parentPids)
        // {
        //     cout << element << " ";
        // }
        // cout << endl;


    }





    return 0;
}