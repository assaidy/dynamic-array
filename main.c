#include <stdio.h>
#include "./da.h"

void printDA(DynamicArray *da) {
    for (int i = 0; i < da->len; i++) {
        printf("%d ", da->arr[i]);
    }
    printf("\n");
}

int main() {
    DynamicArray *da = NewDynamicArray(0);
    if (DaError != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printf("cap: %ld\n", da->cap);
    printf("len: %ld\n", da->len);
    printf("empty: %d\n", DaEmpty(da));

    DaPop(da);
    if (DaError != DaNoError) {
        printf("failed to pop due to: %s\n", DaGetErrMsg());
    }

    DaAppend(da, 10);
    if (DaError != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printDA(da);
    printf("cap: %ld\n", da->cap);
    printf("len: %ld\n", da->len);
    printf("empty: %d\n", DaEmpty(da));

    DaAppend(da, 20);
    DaAppend(da, 30);
    DaAppend(da, 40);
    DaAppend(da, 50);
    DaAppend(da, 60);
    if (DaError != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printDA(da);
    printf("cap: %ld\n", da->cap);
    printf("len: %ld\n", da->len);
    printf("empty: %d\n", DaEmpty(da));

    DaDestroy(&da);
    if (DaError != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }
    printf("%d\n", da == NULL);

    return 0;
}
