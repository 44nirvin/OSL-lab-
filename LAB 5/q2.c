#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, rt;
    int ct, tat, wt;
};

void calculate(struct Process *p, int n) {
    for (int i = 0; i < n; i++) {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
}

int main() {

    int n = 4;
    struct Process *p = malloc(n * sizeof(struct Process));

    p[0] = (struct Process){1,0,20,20,0,0,0};
    p[1] = (struct Process){2,1,25,25,0,0,0};
    p[2] = (struct Process){3,2,10,10,0,0,0};
    p[3] = (struct Process){4,3,15,15,0,0,0};

    int time = 0;
    int q1 = 4, q2 = 8;

    for (int i = 0; i < n; i++) {

        if (time < p[i].at)
            time = p[i].at;

        /* -------- Q1 (RR 4) -------- */
        if (p[i].rt > q1) {
            p[i].rt -= q1;
            time += q1;
        } else {
            time += p[i].rt;
            p[i].rt = 0;
            p[i].ct = time;
            continue;
        }

        /* -------- Q2 (RR 8) -------- */
        if (p[i].rt > q2) {
            p[i].rt -= q2;
            time += q2;
        } else {
            time += p[i].rt;
            p[i].rt = 0;
            p[i].ct = time;
            continue;
        }

        /* -------- Q3 (FCFS) -------- */
        time += p[i].rt;
        p[i].rt = 0;
        p[i].ct = time;
    }

    calculate(p, n);

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].at, p[i].bt,
               p[i].ct, p[i].tat, p[i].wt);

    free(p);
    return 0;
}
