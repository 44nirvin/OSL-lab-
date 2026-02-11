#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
    int pid, at, bt, rt, deadline;
    int ct, tat, wt;
};

int main() {

    int n = 4;
    struct Process *p = malloc(n * sizeof(struct Process));

    p[0] = (struct Process){1,0,3,3,7,0,0,0};
    p[1] = (struct Process){2,2,6,6,12,0,0,0};
    p[2] = (struct Process){3,4,4,4,10,0,0,0};
    p[3] = (struct Process){4,6,5,5,15,0,0,0};

    int completed = 0, time = 0;

    while (completed < n) {

        int idx = -1;
        int earliest_deadline = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].rt > 0) {
                if (p[i].deadline < earliest_deadline) {
                    earliest_deadline = p[i].deadline;
                    idx = i;
                }
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

    printf("\nPID\tAT\tBT\tDL\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt, p[i].deadline,
               p[i].ct, p[i].tat, p[i].wt);

    free(p);
    return 0;
}
