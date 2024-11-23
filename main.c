#include <stdio.h>
#define DA_IMPLEMENTATION
#include "./da.h"

void printIntDA(DynamicArray *da) {
    for (size_t i = 0; i < da->len; i++) {
        printf("%d ", *(int *)daGet(da, i));
    }
    printf("\n");
}

void printFloatDA(DynamicArray *da) {
    for (size_t i = 0; i < da->len; i++) {
        printf("%.2f ", *(float *)daGet(da, i));
    }
    printf("\n");
}

int compareFloat(void *a, void *b) {
    return *(float *)a - *(float *)b;
}

int main() {
    printf("\n=============================== test with int ===============================\n");
    DynamicArray daInt = {0};
    daInit(&daInt, 0, sizeof(int));
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    printf("cap: %ld\n", daInt.cap);
    printf("len: %ld\n", daInt.len);
    printf("empty: %d\n", daEmpty(&daInt));

    void *_ = daPop(&daInt);
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    daAppend(&daInt, &(int){10});
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    printf("cap: %ld\n", daInt.cap);
    printf("len: %ld\n", daInt.len);
    printf("empty: %d\n", daEmpty(&daInt));
    printIntDA(&daInt);

    daAppend(&daInt, &(int){20});
    daAppend(&daInt, &(int){30});
    daAppend(&daInt, &(int){40});
    daAppend(&daInt, &(int){50});
    daAppend(&daInt, &(int){60});
    if (daGetError() != daNoError) {
        printf("there is an error: %d\n", __LINE__);
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    printIntDA(&daInt);

    size_t idx = daIndex(&daInt, &(int){7});
    printf("idx of 7: %ld\n", idx);
    idx = daIndex(&daInt, &(int){30});
    printf("idx of 30: %ld\n", idx);

    daReverse(&daInt);

    printIntDA(&daInt);
    printf("cap: %ld\n", daInt.cap);
    printf("len: %ld\n", daInt.len);
    printf("empty: %d\n", daEmpty(&daInt));

    daDestroy(&daInt);
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }
    printf("destroyed: %d\n", daInt.arr == NULL);

    printf("\n=============================== test with float ===============================\n");

    DynamicArray daFloat = {0};
    daInit(&daFloat, 0, sizeof(float));
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    printf("cap: %ld\n", daFloat.cap);
    printf("len: %ld\n", daFloat.len);
    printf("empty: %d\n", daEmpty(&daFloat));

    _ = daPop(&daFloat);
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    printf("passed\n");

    daAppend(&daFloat, &(float){10.5});
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    printFloatDA(&daFloat);
    printf("cap: %ld\n", daFloat.cap);
    printf("len: %ld\n", daFloat.len);
    printf("empty: %d\n", daEmpty(&daFloat));

    daAppend(&daFloat, &(float){20.5});
    daAppend(&daFloat, &(float){30.5});
    daAppend(&daFloat, &(float){40.5});
    daAppend(&daFloat, &(float){50.5});
    daAppend(&daFloat, &(float){60.5});
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }

    printFloatDA(&daFloat);

    daInsertAt(&daFloat, 0, &(float){15.0});
    daInsertAt(&daFloat, 2, &(float){1.0});

    printFloatDA(&daFloat);

    float *min = (float *)daMin(&daFloat, compareFloat);
    float *max = (float *)daMax(&daFloat, compareFloat);

    printf("min: %f\n", *min);
    printf("max: %f\n", *max);

    idx = daIndex(&daFloat, &(float){7.0});
    printf("idx of 7.0: %ld\n", idx);
    idx = daIndex(&daFloat, &(float){1.0});
    printf("idx of 1.0: %ld\n", idx);

    daRemoveAt(&daFloat, 1);
    daRemoveAt(&daFloat, daFloat.len - 1);

    printFloatDA(&daFloat);
    printf("cap: %ld\n", daFloat.cap);
    printf("len: %ld\n", daFloat.len);
    printf("empty: %d\n", daEmpty(&daFloat));

    daClear(&daFloat);
    printf("cap: %ld\n", daFloat.cap);
    printf("len: %ld\n", daFloat.len);
    printf("empty: %d\n", daEmpty(&daFloat));

    daDestroy(&daFloat);
    if (daGetError() != daNoError) {
        printf("line:%d: there is an error with number: %d\n", __LINE__, daGetError());
    }
    printf("destroyed: %d\n", daFloat.arr == NULL);

    return 0;
}
