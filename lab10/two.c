#include <stdio.h>
#include <stdlib.h>

int search(int key, int *frames, int f, int *r, int iter){
    for(int i=0;i<f;i++){
        if(frames[i]==key){
            r[i]=iter;
            return 1;
        }
    }
    return 0;
}

int minimum(int *r, int f){
    int min = r[0];
    int index = 0;

    for(int i=1;i<f;i++){
        if(r[i] < min){
            min = r[i];
            index = i;
        }
    }
    return index;
}

void lru(int *pages, int f, int n){
    int *frames = (int*)malloc(f*sizeof(int));
    int *r = (int*)malloc(f*sizeof(int));
    int faults = 0;

    for(int i=0;i<f;i++){
        frames[i] = -1;
        r[i] = 0;
    }

    for(int i=0;i<n;i++){

        if(!search(pages[i],frames,f,r,i)){

            int placed = 0;

            for(int j=0;j<f;j++){
                if(frames[j] == -1){
                    frames[j] = pages[i];
                    r[j] = i;
                    faults++;
                    placed = 1;
                    break;
                }
            }

            if(!placed){
                int least = minimum(r,f);
                frames[least] = pages[i];
                r[least] = i;
                faults++;
            }
        }

        printf("Frames: ");
        for(int j=0;j<f;j++){
            if(frames[j] == -1)
                printf("- ");
            else
                printf("%d ",frames[j]);
        }
        printf("\n");
    }

    printf("Total faults: %d\n",faults);

    free(frames);
    free(r);
}

int main(){
    int n, f;

    printf("Enter number of pages: ");
    scanf("%d",&n);

    int *pages = (int*)malloc(n*sizeof(int));

    printf("Enter reference string:\n");
    for(int i=0;i<n;i++){
        scanf("%d",&pages[i]);
    }

    printf("Enter number of frames: ");
    scanf("%d",&f);

    printf("\nImplementing LRU:\n\n");

    lru(pages, f, n);

    free(pages);

    return 0;
}