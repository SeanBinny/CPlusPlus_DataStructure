#ifndef FACTORYSIMULATION_TEST_H
#define FACTORYSIMULATION_TEST_H
#include "queue.h"

struct task
{
    int machine;
    int time;

    task(int theMachine = 0, int theTime = 0)
    {
        machine = theMachine;
        time    = theTime;
    }
};

struct job
{
    arrayQueue<task> taskQ;
    int length;
    int arrivalTime;
    int id;

    job(int theId = 0)
    {
        id          = theId;
        length      = 0;
        arrivalTime = 0;
    }
    void addTask(int theMachine, int theTime)
    {
        task theTask(theMachine, theTime);
        taskQ.push(theTask);
    }
    int removeNextTask()
    {
        int theTime = taskQ.front().time;
        taskQ.pop();
        length += theTime;
        return theTime;
    }
};

struct machine
{
    arrayQueue<job*> jobQ;

    int  changeTime;
    int  totalWait;
    int  numTasks;
    job *activeJob;

    machine()
    {
        totalWait = 0;
        numTasks  = 0;
        activeJob = NULL;
    }
};

class eventList
{
public:
    eventList(int theNumMachines, int theLargeTime)
    {
        if (theNumMachines < 2)
            throw illegalParameterValue("number of machines must be > 1");
        numMachines = theNumMachines;
        finishTime  = new int [numMachines+1];
        for (int i = 1; i <= numMachines; i++ )
            finishTime[i] = theLargeTime;
    }
    int nextEventMachine()
    {
        int p = 1;
        int t = finishTime[1];
        for (int i = 2; i <= numMachines; i++)
        {
            if (finishTime[i] < t)
            {
                p = i;
                t = finishTime[i];
            }
        }
        return p;
    }
    int nextEventTime(int theMachine) {return finishTime[theMachine];}
    void setFinishTime(int theMachine, int theTime)
    {
        finishTime[theMachine] = theTime;
    }

private:
    int* finishTime;
    int  numMachines;
};

int timeNow;
int numMachines;
int numJobs;
int largeTime = 100;
eventList* eList;
machine*   mArray;

void inputData()
{
    cout << "Enter number of machines and jobs" << endl;
    cin  >> numMachines >> numJobs;
    if (numMachines < 1 || numJobs < 1)
        throw illegalParameterValue("number of machines and jobs "
                                    "must be >= 1");

    eList  = new eventList(numMachines, largeTime);
    mArray = new machine[numMachines+1];

    cout << "Enter change-over times for machines" << endl;
    int ct;
    for (int j = 1; j <= numMachines; j++)
    {
        cin >> ct;
        if (ct < 0)
            throw illegalParameterValue("change-over time must be "
                                        ">= 0");
        mArray[j].changeTime = ct;
    }

    job* theJob;
    int  numTasks, firstMachine, theMachine, theTaskTime;
    for (int i = 1; i <= numJobs; i++)
    {
        cout << "Enter number of tasks for job " << i << endl;
        cin  >> numTasks;
        firstMachine = 0;
        if (numTasks < 1)
            throw illegalParameterValue("each job must have > 1 task");

        theJob = new job(i);
        cout << "Enter the tasks (machine, time)"
             << " in process order" << endl;
    }
    for (int j = 1; j <= numTasks; j++)
    {
        cin >> theMachine >> theTaskTime;
        if (theMachine  < 1 || theMachine > numMachines ||
            theTaskTime < 1)
            throw illegalParameterValue("bad machine number or task time");
        if (j == 1)
            firstMachine = theMachine;
        theJob->addTask(theMachine, theTaskTime);
    }
    mArray[firstMachine].jobQ.push(theJob);
}

job* changeStates(int theMachine)
{
    job* lastJob;
    if (mArray[theMachine].activeJob == NULL)
    {
        lastJob = NULL;
        if (mArray[theMachine].jobQ.empty())
            eList->setFinishTime(theMachine, largeTime);
        else
        {
            mArray[theMachine].activeJob = mArray[theMachine].jobQ.front();
            mArray[theMachine].jobQ.pop();
            mArray[theMachine].totalWait +=
                timeNow-mArray[theMachine].activeJob->removeNextTask();
            mArray[theMachine].numTasks++;

            int t = mArray[theMachine].activeJob->removeNextTask();
            eList->setFinishTime(theMachine, timeNow+t);
        }
    }
    else
    {
        lastJob = mArray[theMachine].activeJob;
        mArray[theMachine].activeJob = NULL;
        eList->setFinishTime(theMachine, timeNow+mArray[theMachine].changeTime);
    }

    return lastJob;
}

void startShop()
{
    for (int p = 1; p <= numMachines; p++)
        changeStates(p);
}

bool moveToNextMachine(job* theJob)
{
    if (theJob->taskQ.empty())
    {
        cout << "Job" << theJob->id << " has completed at "
             << timeNow << " Total wait was "
             << (timeNow-theJob->length) << endl;
        return false;
    }
    else
    {

        int p = theJob->taskQ.front().machine;
        mArray[p].jobQ.push(theJob);
        theJob->arrivalTime = timeNow;
        if (eList->nextEventTime(p) == largeTime)
            changeStates(p);

        return true;
    }
}

void simulate()
{
    while (numJobs > 0)
    {
        int nextToFinish = eList->nextEventMachine();
        timeNow          = eList->nextEventTime(nextToFinish);

        job* theJob      = changeStates(nextToFinish);
        if (theJob != NULL && !moveToNextMachine(theJob))
            numJobs--;
    }
}

void outputStatistics()
{
    cout << "Finish time = " << timeNow << endl;
    for (int p = 1; p <= numMachines; p++)
    {
        cout << "Machine " << p << " completed "
             << mArray[p].numTasks << " tasks" << endl;
        cout << "The total wait time was "
             << mArray[p].totalWait << endl;
        cout << endl;
    }
}

void main_Example ()
{
    inputData();
    startShop();
    simulate();
    outputStatistics();
}
#endif // FACTORYSIMULATION_TEST_H
