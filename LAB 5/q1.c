#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
    int pid, at, bt, rt, priority;
    int ct, tat, wt;
};

void reset(struct Process *p, int n) {
    for (int i = 0; i < n; i++) {
        p[i].rt = p[i].bt;
        p[i].ct = p[i].tat = p[i].wt = 0;
    }
}

void printResults(struct Process *p, int n) {
    float total_wt = 0, total_tat = 0;

    printf("\nPID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].priority,
               p[i].ct, p[i].tat, p[i].wt);

        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }

    printf("Average WT = %.2f\n", total_wt / n);
    printf("Average TAT = %.2f\n", total_tat / n);
}

/* ---------------- FCFS ---------------- */

void fcfs(struct Process *p, int n) {
    int time = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at)
            time = p[i].at;

        time += p[i].bt;

        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
}

/* ---------------- SRTF ---------------- */

void srtf(struct Process *p, int n) {
    int completed = 0, time = 0;

    while (completed < n) {
        int idx = -1, min_rt = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0 && p[i].rt < min_rt) {
                min_rt = p[i].rt;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].rt--;
        time++;

        if (p[idx].rt == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            completed++;
        }
    }
}

/* ---------------- Round Robin ---------------- */

void roundRobin(struct Process *p, int n, int quantum) {
    int queue[100], front = 0, rear = 0;
    int visited[100] = {0};
    int completed = 0, time = 0;

    queue[rear++] = 0;
    visited[0] = 1;

    while (completed < n) {

        int idx = queue[front++];

        if (p[idx].rt > quantum) {
            p[idx].rt -= quantum;
            time += quantum;
        } else {
            time += p[idx].rt;
            p[idx].rt = 0;

            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            completed++;
        }

        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].at <= time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].rt > 0)
            queue[rear++] = idx;

        if (front == rear) {
            time++;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && p[i].at <= time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
        }
    }
}

/* ---------------- Non-Preemptive Priority ---------------- */

void priorityNP(struct Process *p, int n) {
    int completed = 0, time = 0;
    int done[100] = {0};

    while (completed < n) {
        int idx = -1, highest = -1;

        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].priority > highest) {
                highest = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        time += p[idx].bt;

        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;

        done[idx] = 1;
        completed++;
    }
}

/* ---------------- MAIN ---------------- */

int main() {

    int n = 4;
    struct Process *p = malloc(n * sizeof(struct Process));

    p[0] = (struct Process){1,0,60,60,3,0,0,0};
    p[1] = (struct Process){2,3,30,30,2,0,0,0};
    p[2] = (struct Process){3,4,40,40,1,0,0,0};
    p[3] = (struct Process){4,9,10,10,4,0,0,0};

    printf("\n===== FCFS =====\n");
    reset(p, n);
    fcfs(p, n);
    printResults(p, n);

    printf("\n===== SRTF =====\n");
    reset(p, n);
    srtf(p, n);
    printResults(p, n);

    printf("\n===== Round Robin (q=10) =====\n");
    reset(p, n);
    roundRobin(p, n, 10);
    printResults(p, n);

    printf("\n===== Non-Preemptive Priority =====\n");
    reset(p, n);
    priorityNP(p, n);
    printResults(p, n);

    free(p);
    return 0;
}
