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

int compareFloat(void *a, void *b) {
    return *(float *)a - *(float *)b;
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

    DaAppend(daInt, &(int){10});
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printIntDA(daInt);
    printf("cap: %ld\n", daInt->cap);
    printf("len: %ld\n", daInt->len);
    printf("empty: %d\n", DaEmpty(daInt));

    DaAppend(daInt, &(int){20});
    DaAppend(daInt, &(int){30});
    DaAppend(daInt, &(int){40});
    DaAppend(daInt, &(int){50});
    DaAppend(daInt, &(int){60});
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printIntDA(daInt);

    size_t idx = DaIndex(daInt, &(int){7});
    printf("idx of 7.0: %ld\n", idx);
    idx = DaIndex(daInt, &(int){30});
    printf("idx of 10.0: %ld\n", idx);

    DaReverse(daInt);

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

    DaAppend(daFloat, &(float){10.5});
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printFloatDA(daFloat);
    printf("cap: %ld\n", daFloat->cap);
    printf("len: %ld\n", daFloat->len);
    printf("empty: %d\n", DaEmpty(daFloat));

    DaAppend(daFloat, &(float){20.5});
    DaAppend(daFloat, &(float){30.5});
    DaAppend(daFloat, &(float){40.5});
    DaAppend(daFloat, &(float){50.5});
    DaAppend(daFloat, &(float){60.5});
    if (DaGetError() != DaNoError) {
        printf("there is an error: %s\n", DaGetErrMsg());
    }

    printFloatDA(daFloat);

    DaInsertAt(daFloat, 0, &(float){15.0});
    DaInsertAt(daFloat, 2, &(float){1.0});

    printFloatDA(daFloat);

    float *min = (float *)DaMin(daFloat, compareFloat);
    float *max = (float *)DaMax(daFloat, compareFloat);

    printf("min: %f\n", *min);
    printf("max: %f\n", *max);

    idx = DaIndex(daFloat, &(float){7.0});
    printf("idx of 7.0: %ld\n", idx);
    idx = DaIndex(daFloat, &(float){1.0});
    printf("idx of 1.0: %ld\n", idx);

    DaRemoveAt(daFloat, 1);
    DaRemoveAt(daFloat, daFloat->len - 1);

    printFloatDA(daFloat);
    printf("cap: %ld\n", daFloat->cap);
    printf("len: %ld\n", daFloat->len);
    printf("empty: %d\n", DaEmpty(daFloat));

    DaClear(&daFloat);
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
