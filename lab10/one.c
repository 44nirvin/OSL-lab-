#include <stdio.h>
#include <stdlib.h>

int search(int key, int *frames, int f){
    for(int i=0;i<f;i++){
        if(frames[i]==key){
            return 1;
        }
    }
    return 0;
}
int findOptimal(int *pages, int *frames, int n, int index, int f)
{
    int farthest = index;
    int pos = -1;

    for(int i = 0; i < f; i++){
        int j;
        for(j = index; j < n; j++){
            if(frames[i] == pages[j]){
                if(j > farthest){
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }

        if(j == n)
            return i;
    }

    if(pos == -1)
        return 0;
    else
        return pos;
}
void fifo(int *pages, int n, int f){
    int *frames = (int*)malloc(f * sizeof(int));
    int p = 0;
    int faults = 0;

    for(int i=0;i<f;i++){
        frames[i] = -1;
    }

    printf("\nImplementing FIFO:\n\n");

    for(int i=0;i<n;i++){

        if(!search(pages[i],frames,f)){
            frames[p] = pages[i];
            p = (p+1) % f;
            faults++;
        }

        printf("Frames: ");
        for(int j=0;j<f;j++){
            if(frames[j]==-1)
                printf("- ");
            else
                printf("%d ",frames[j]);
        }
        printf("\n");
    }

    printf("Total faults: %d\n",faults);

    free(frames);
}
void OPTIMAL(int *pages, int n, int f)
{
    int *frames = (int*)malloc(f * sizeof(int));
    int faults = 0;

    for(int i = 0; i < f; i++)
        frames[i] = -1;

    printf("\nOptimal Page Replacement\n");

    for(int i = 0; i < n; i++)
    {
        if(!search(pages[i], frames, f))
        {
            int pos = findOptimal(pages, frames, n, i + 1, f);
            frames[pos] = pages[i];
            faults++;
        }

        printf("Frames: ");
        for(int j = 0; j < f; j++)
            printf("%d ", frames[j]);
        printf("\n");
    }

    printf("Total Page Faults = %d\n", faults);

    free(frames);
}
int main(){
    int f,n;

    printf("Enter number of pages: ");
    scanf("%d",&n);

    int *pages = (int*)malloc(n * sizeof(int));

    printf("Enter reference string:\n");
    for(int i=0;i<n;i++)
        scanf("%d",&pages[i]);

    printf("Enter number of frames: ");
    scanf("%d",&f);

    fifo(pages,n,f);
    OPTIMAL(pages, n, f);
    free(pages);
    return 0;
}