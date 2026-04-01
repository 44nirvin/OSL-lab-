#include <stdio.h>

#define N 5

struct Resources {
    int A, B, C;
};

struct Process {
    struct Resources allocation;
    struct Resources max;
    struct Resources need;
    int finish;
};

int canRun(struct Resources need, struct Resources work) {
    if(need.A <= work.A &&
       need.B <= work.B &&
       need.C <= work.C)
        return 1;

    return 0;
}

int main() {

    int i, count;

    // Separate Allocation
    struct Resources Allocation[N] = {
        {0,1,0},
        {2,0,0},
        {3,0,2},
        {2,1,1},
        {0,0,2}
    };

    // Separate Max
    struct Resources Max[N] = {
        {7,5,3},
        {3,2,2},
        {9,0,2},
        {2,2,2},
        {4,3,3}
    };

    struct Process p[N];

    struct Resources Available = {3,3,2};
    struct Resources Work;

    int safeSeq[N];
    int index = 0;

    // Manual Initialization (VERY IMPORTANT)
    for(i = 0; i < N; i++) {

        p[i].allocation.A = Allocation[i].A;
        p[i].allocation.B = Allocation[i].B;
        p[i].allocation.C = Allocation[i].C;

        p[i].max.A = Max[i].A;
        p[i].max.B = Max[i].B;
        p[i].max.C = Max[i].C;

        p[i].need.A = p[i].max.A - p[i].allocation.A;
        p[i].need.B = p[i].max.B - p[i].allocation.B;
        p[i].need.C = p[i].max.C - p[i].allocation.C;

        p[i].finish = 0;
    }

    printf("Need Matrix:\n");
    for(i = 0; i < N; i++) {
        printf("P%d: %d %d %d\n",
               i,
               p[i].need.A,
               p[i].need.B,
               p[i].need.C);
    }

    // Work Initialization
    Work.A = Available.A;
    Work.B = Available.B;
    Work.C = Available.C;

    // Safety Algorithm
    for(count = 0; count < N; count++) {

        int found = 0;

        for(i = 0; i < N; i++) {

            if(p[i].finish == 0 && canRun(p[i].need, Work)) {

                Work.A += p[i].allocation.A;
                Work.B += p[i].allocation.B;
                Work.C += p[i].allocation.C;

                p[i].finish = 1;
                safeSeq[index++] = i;

                found = 1;
            }
        }

        if(found == 0)
            break;
    }

    // Final Check
    int safe = 1;

    for(i = 0; i < N; i++) {
        if(p[i].finish == 0) {
            safe = 0;
            break;
        }
    }

    if(safe) {
        printf("\nSystem is in SAFE state\n");
        printf("Safe Sequence: ");

        for(i = 0; i < N; i++)
            printf("P%d ", safeSeq[i]);
    }
    else {
        printf("\nSystem is NOT SAFE\n");
    }

    return 0;
}