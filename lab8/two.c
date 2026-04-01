#include <stdio.h>

#define N 5

struct Resources {
    int A, B, C;
};

struct Process {
    struct Resources allocation;
    struct Resources request;
    int finish;
};

int canRun(struct Resources req, struct Resources work) {
    if(req.A <= work.A &&
       req.B <= work.B &&
       req.C <= work.C)
        return 1;

    return 0;
}

int main() {

    int i, count;

    struct Process p[N] = {

        {{0,1,0}, {0,0,0}},   // P0
        {{2,0,0}, {2,0,2}},   // P1
        {{3,0,3}, {0,0,0}},   // P2
        {{2,1,1}, {1,0,0}},   // P3
        {{0,0,2}, {0,0,2}}    // P4
    };

    struct Resources Available = {0,0,0};
    struct Resources Work;

    Work.A = Available.A;
    Work.B = Available.B;
    Work.C = Available.C;

    // Initialize Finish
    for(i = 0; i < N; i++) {
        if(p[i].allocation.A == 0 &&
           p[i].allocation.B == 0 &&
           p[i].allocation.C == 0)
            p[i].finish = 1;
        else
            p[i].finish = 0;
    }

    // Detection Algorithm
    for(count = 0; count < N; count++) {

        int found = 0;

        for(i = 0; i < N; i++) {

            if(p[i].finish == 0 && canRun(p[i].request, Work)) {

                Work.A += p[i].allocation.A;
                Work.B += p[i].allocation.B;
                Work.C += p[i].allocation.C;

                p[i].finish = 1;
                found = 1;
            }
        }

        if(found == 0)
            break;
    }

    // Check Deadlock
    int deadlock = 0;

    for(i = 0; i < N; i++) {
        if(p[i].finish == 0) {
            deadlock = 1;
            printf("Process P%d is DEADLOCKED\n", i);
        }
    }

    if(!deadlock)
        printf("\nNo Deadlock Detected (System Safe)\n");

    return 0;
}