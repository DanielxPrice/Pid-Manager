#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <string.h>

#define MIN_PID 100
#define MAX_PID 1000
#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1


#include "PIDCLASS.h"

using namespace std;

int main() 
{
    /***********************************************************************/
    // Pipe Set up
    char pid_msg[BUFFER_SIZE] = "Need PID";
    char deallocate_pid[BUFFER_SIZE] = "Deallocate PID";
    char done_msg[BUFFER_SIZE] = "Done";

    char read_msg[BUFFER_SIZE];

    int parentToChild[2];
    int childToParent[2];

    

    /* create the pipe */

    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) 
    {
    
        fprintf(stderr,"Pipe failed");
        return 1;
    }



    /***********************************************************************/


    vector<int> parentPids;
    //vector<int> childPids;

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
        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);

        for (int i = 0; i < 3; ++i)
        {
            // Writing to request PID from parent
            write(childToParent[WRITE_END], pid_msg, strlen(pid_msg)+1);
            sleep(4);

            read(parentToChild[READ_END], read_msg, BUFFER_SIZE);
            cout << "Hello from child, received PID: " << read_msg << endl;

            write(childToParent[WRITE_END], deallocate_pid, strlen(deallocate_pid)+1);
            sleep(4);
        }
        write(childToParent[WRITE_END], done_msg, strlen(done_msg) + 1);

        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
    }
    //PARENT PROCESS
    else 
    {
        //wait(NULL);
        PidManager parentPidManager;
        
        close(parentToChild[READ_END]);
        close(childToParent[WRITE_END]);
    
        while (true)
        {
            // Read request from child
            read(childToParent[READ_END], read_msg, BUFFER_SIZE);
            printf("\nParent received request: %s", read_msg);

            // If child is done, break the loop
            if (strcmp(read_msg, "Done") == 0)
            {
                cout << endl;
                break;
            }
            else if (strcmp(read_msg, pid_msg) == 0)
            {
                // Allocate PID
                int ppid = parentPidManager.allocate_pid();
                cout << "\nParent created PID: " << ppid << endl;
                parentPids.push_back(ppid);

                // Send PID to child
                string pid_str = to_string(ppid);
                write(parentToChild[WRITE_END], pid_str.c_str(), pid_str.length() + 1);
            }
            else if (strcmp(read_msg, deallocate_pid) == 0)
            {
                // Deallocate the last allocated PID
                if (!parentPids.empty())
                {
                    int ppid = parentPids.back();
                    printf("\nParent received request to release PID: %d", ppid);
                    parentPidManager.release_pid(ppid);
                    cout << "Parent deallocated PID: " << ppid << endl << endl;
                    parentPids.pop_back();
                }
            }
        }
        wait(NULL);
        close(parentToChild[WRITE_END]);
        close(childToParent[READ_END]);

    }





    return 0;
}