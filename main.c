#include <stdio.h>
#include "./da.h"

void printIntDA(DynamicArray *da) {
    for (size_t i = 0; i < da->len; i++) {
        int item;
        DaGet(da, i, &item);
        printf("%d ", item);
    }
    printf("\n");
}

void printFloatDA(DynamicArray *da) {
    for (size_t i = 0; i < da->len; i++) {
        float item;
        DaGet(da, i, &item);
        printf("%.2f ", item);
    }
    printf("\n");
}

int main() {
    printf("\n=============================== test with int ===============================\n");
    DynamicArray *daInt = NewDynamicArray(0, sizeof(int));
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printf("cap: %ld\n", daInt->cap);
    printf("len: %ld\n", daInt->len);
    printf("empty: %d\n", DaEmpty(daInt));

    int item;
    DaPop(daInt, &item);
    if (DaGetError() != DaNoError) {
        printf("failed to pop due to: %s\n", DaGetErrMsg());
    }

    DaAppend(daInt, DaElem(int, 10));
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printIntDA(daInt);
    printf("cap: %ld\n", daInt->cap);
    printf("len: %ld\n", daInt->len);
    printf("empty: %d\n", DaEmpty(daInt));

    DaAppend(daInt, DaElem(int, 20));
    DaAppend(daInt, DaElem(int, 30));
    DaAppend(daInt, DaElem(int, 40));
    DaAppend(daInt, DaElem(int, 50));
    DaAppend(daInt, DaElem(int, 60));
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printIntDA(daInt);
    printf("cap: %ld\n", daInt->cap);
    printf("len: %ld\n", daInt->len);
    printf("empty: %d\n", DaEmpty(daInt));

    DaDestroy(&daInt);
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }
    printf("destroyed: %d\n", daInt == NULL);

    printf("\n=============================== test with float ===============================\n");

    DynamicArray *daFloat = NewDynamicArray(0, sizeof(float));
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printf("cap: %ld\n", daFloat->cap);
    printf("len: %ld\n", daFloat->len);
    printf("empty: %d\n", DaEmpty(daFloat));

    float output;
    DaPop(daFloat, &output);
    if (DaGetError() != DaNoError) {
        printf("failed to pop due to: %s\n", DaGetErrMsg());
    }

    DaAppend(daFloat, DaElem(float, 10.5));
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printFloatDA(daFloat);
    printf("cap: %ld\n", daFloat->cap);
    printf("len: %ld\n", daFloat->len);
    printf("empty: %d\n", DaEmpty(daFloat));

    DaAppend(daFloat, DaElem(float, 20.5));
    DaAppend(daFloat, DaElem(float, 30.5));
    DaAppend(daFloat, DaElem(float, 40.5));
    DaAppend(daFloat, DaElem(float, 50.5));
    DaAppend(daFloat, DaElem(float, 60.5));
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printFloatDA(daFloat);
    printf("cap: %ld\n", daFloat->cap);
    printf("len: %ld\n", daFloat->len);
    printf("empty: %d\n", DaEmpty(daFloat));

    DaDestroy(&daFloat);
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }
    printf("destroyed: %d\n", daFloat == NULL);

    return 0;
}
