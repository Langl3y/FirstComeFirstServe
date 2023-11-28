#include <stdio.h>
#include <stdlib.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]);
void printProcess(int n, PCB P[]);
void exportGanttChart(int n, PCB P[]);
void pushProcess(int *n, PCB P[], PCB Q);
void removeProcess(int *n, int index, PCB P[]);
int swapProcess(PCB *P, PCB *Q);
int partition(PCB P[], int low, int high, int iCriteria);
void quickSort(PCB P[], int low, int high, int iCriteria);
void calculateAWT(int n, PCB P[]);
void calculateATaT(int n, PCB P[]);

int main() {
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;

    printf("Please input the number of processes: ");
    scanf("%d", &iNumberOfProcess);

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    while (iTerminated < iNumberOfProcess) {
        while (iRemain > 0) {
            if (Input[0].iArrival <= ReadyQueue[0].iFinish) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                continue;
            } else {
                break;
            }
        }

        if (iReady > 0) {
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);
            ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
        }
    }

    printf("\n===== FCFS Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}

void inputProcess(int n, PCB P[]) {
    int i;
    for (i = 0; i < n; i++) {
        printf("\nEnter details for Process %d:\n", i + 1);
        printf("PID: ");
        scanf("%d", &P[i].iPID);
        printf("Arrival Time: ");
        scanf("%d", &P[i].iArrival);
        printf("Burst Time: ");
        scanf("%d", &P[i].iBurst);
    }
}

void printProcess(int n, PCB P[]) {
    int i;
    for (i = 0; i < n; i++) {
        printf("\nPID: %d", P[i].iPID);
        printf("\nArrival Time: %d", P[i].iArrival);
        printf("\nBurst Time: %d", P[i].iBurst);
    }
    printf("\n");
}

void exportGanttChart(int n, PCB P[]) {
    printf("\nGantt Chart:\n");
    printf("--------------------------------------------------\n");
    printf("| Process |   Start   |   Finish  |   Duration   |\n");
    printf("--------------------------------------------------\n");

    int i;
    for (i = 0; i < n; i++) {
        printf("|   P%d    |    %d     |    %d     |     %d      |\n",
               P[i].iPID,
               P[i].iStart,
               P[i].iFinish,
               P[i].iFinish - P[i].iStart);
        printf("--------------------------------------------------\n");
    }
}

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    int i;
    for (i = index; i < *n - 1; i++) {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
}

int partition(PCB P[], int low, int high, int iCriteria) {
    int pivot = P[high].iPID;
    int i = (low - 1);

    int j;
    for (j = low; j <= high - 1; j++) {
        if (iCriteria == SORT_BY_ARRIVAL) {
            if (P[j].iArrival <= pivot) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        } else if (iCriteria == SORT_BY_PID) {
            if (P[j].iPID <= pivot) {
                i++;
                swapProcess(&P[i], &P[j]);
            }
        }
        /* Handle other sorting criteria here */

    }
    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);

        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
    int i;
    float avgWaitingTime = 0;

    for (i = 0; i < n; i++) {
        P[i].iWaiting = P[i].iTaT - P[i].iBurst;
        avgWaitingTime += P[i].iWaiting;
    }

    avgWaitingTime /= n;

    printf("\nAverage Waiting Time: %.2f\n", avgWaitingTime);
}

void calculateATaT(int n, PCB P[]) {
    int i;
    float avgTurnaroundTime = 0;

    for (i = 0; i < n; i++) {
        avgTurnaroundTime += P[i].iTaT;
    }

    avgTurnaroundTime /= n;

    printf("Average Turnaround Time: %.2f\n", avgTurnaroundTime);
}