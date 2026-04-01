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

    // Initial Allocation
    struct Resources Allocation[N] = {
        {0,1,0},
        {2,0,0},
        {3,0,2},
        {2,1,1},
        {0,0,2}
    };

    // Max Matrix
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

    // Initialization
    for(i = 0; i < N; i++) {

        p[i].allocation = Allocation[i];
        p[i].max = Max[i];

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

    // -------- REQUEST SECTION --------

    struct Resources Request;
    int pid;

    printf("\nEnter Process ID making request: ");
    scanf("%d", &pid);

    printf("Enter Request (A B C): ");
    scanf("%d %d %d", &Request.A, &Request.B, &Request.C);

    // Check Request ≤ Need
    if(Request.A > p[pid].need.A ||
       Request.B > p[pid].need.B ||
       Request.C > p[pid].need.C)
    {
        printf("Error: Request exceeds Need\n");
        return 0;
    }

    // Check Request ≤ Available
    if(Request.A > Available.A ||
       Request.B > Available.B ||
       Request.C > Available.C)
    {
        printf("Resources unavailable → Process must WAIT\n");
        return 0;
    }

    // Pretend Allocation
    Available.A -= Request.A;
    Available.B -= Request.B;
    Available.C -= Request.C;

    p[pid].allocation.A += Request.A;
    p[pid].allocation.B += Request.B;
    p[pid].allocation.C += Request.C;

    p[pid].need.A -= Request.A;
    p[pid].need.B -= Request.B;
    p[pid].need.C -= Request.C;

    // -------- SAFETY CHECK --------

    // Reset Finish & Work
    for(i = 0; i < N; i++)
        p[i].finish = 0;

    Work = Available;
    index = 0;

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

    // Final Safety Decision
    int safe = 1;

    for(i = 0; i < N; i++) {
        if(p[i].finish == 0) {
            safe = 0;
            break;
        }
    }

    if(safe) {
        printf("\nRequest GRANTED\n");

        printf("Updated Available: %d %d %d\n",
               Available.A, Available.B, Available.C);

        printf("\nUpdated Allocation:\n");
        for(i = 0; i < N; i++) {
            printf("P%d: %d %d %d\n",
                   i,
                   p[i].allocation.A,
                   p[i].allocation.B,
                   p[i].allocation.C);
        }

        printf("\nUpdated Need:\n");
        for(i = 0; i < N; i++) {
            printf("P%d: %d %d %d\n",
                   i,
                   p[i].need.A,
                   p[i].need.B,
                   p[i].need.C);
        }

        printf("\nSafe Sequence: ");
        for(i = 0; i < N; i++)
            printf("P%d ", safeSeq[i]);
    }
    else {

        printf("\nRequest DENIED (Unsafe State) → Rolling Back\n");

        // Rollback
        Available.A += Request.A;
        Available.B += Request.B;
        Available.C += Request.C;

        p[pid].allocation.A -= Request.A;
        p[pid].allocation.B -= Request.B;
        p[pid].allocation.C -= Request.C;

        p[pid].need.A += Request.A;
        p[pid].need.B += Request.B;
        p[pid].need.C += Request.C;
    }

    return 0;
}