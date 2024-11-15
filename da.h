#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// TODO: other features
// [ ] use different types than int.
// [ ] DaClear
// [ ] DaInsertAt
// [ ] DaRemoveAt
// [ ] DaReverse
// [ ] DaFind -> takes a predicate func
// [ ] DaSort -> takes a comparison func
// [ ] DaMin -> takes a comparison func
// [ ] DaMax -> takes a comparison func
// [ ] DaCopy(DynamicArray *src, DynamicArray *dist) -> deep copy

typedef struct {
    size_t cap;
    size_t len;
    int *arr;
} DynamicArray;

typedef enum {
    DaNoError,
    DaIsEmptyError,
    DaIsNULLError,
    DaAllocFailedError,
    DaOutOfRangeError,
} DaErrorType;

// FIX: move it to da.c and make it static
// only get it with DaGetError()
DaErrorType DaError = DaNoError;

DaErrorType DaGetError() {
    return DaError;
}

const char *DaGetErrMsg() {
    switch (DaError) {
        case DaNoError: return "no error"; break;
        case DaIsEmptyError: return "da is empty"; break;
        case DaIsNULLError: return "da points to NULL"; break;
        case DaAllocFailedError: return "heap allocation failed for da"; break;
        case DaOutOfRangeError: return "index out of range"; break;
        default: return "invalid error type"; break;
    }
}

// create a new dynamic array on the heap.
// if argument cap is set to 0, the default capcity (i.e. 5) will be used.
// possible errors: 
// - DaIsNULLError
// - DaAllocFailedError
void *NewDynamicArray(size_t cap) {
    DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (da == NULL) {
        DaError = DaAllocFailedError;
        return NULL;
    }
    if (cap == 0) {
        cap = 5;
    }
    int *arr = (int *)malloc(sizeof(int) * cap);
    if (arr == NULL) {
        DaError = DaAllocFailedError;
        free(da);
        return NULL;
    }
    da->arr = arr;
    da->cap = cap;
    da->len = 0;
    DaError = DaNoError;
    return da;
}

// resize the internal array of da and doubling the capacity.
// returns -1 if resize failed.
static int DaResize(DynamicArray *da) {
    da->arr = (int *)realloc(da->arr, sizeof(int) * da->cap * 2);
    if (da->arr == NULL) {
        return -1;
    }
    da->cap *= 2;
    return 0;
}

// append an element at the end of da.
// possible errors: 
// - DaIsNULLError
// - DaAllocFailedError
void DaAppend(DynamicArray *da, int val) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len == da->cap) {
        if (DaResize(da) == -1) {
            DaError = DaAllocFailedError;
            return;
        }
    }
    da->arr[da->len++] = val;
    DaError = DaNoError;
}

// delete and return the last element in da.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
int DaPop(DynamicArray *da) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return 0;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return 0;
    }
    DaError = DaNoError;
    return da->len--; // TODO: might handle shrink
}

// return the first element in da.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
int DaFront(DynamicArray *da) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return 0;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return 0;
    }
    DaError = DaNoError;
    return da->arr[0];
}

// return the last element in da.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
int DaBack(DynamicArray *da) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return 0;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return 0;
    }
    DaError = DaNoError;
    return da->arr[da->len - 1];
}

// get a value in da at idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
int DaGet(DynamicArray *da, size_t idx) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return 0;
    }
    if (da->len <= idx) {
        DaError = DaOutOfRangeError;
        return 0;
    }
    DaError = DaNoError;
    return da->arr[idx];
}

// set a value with val in da at idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
void DaSet(DynamicArray *da, size_t idx, int val) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len <= idx) {
        DaError = DaOutOfRangeError;
        return;
    }
    DaError = DaNoError;
    da->arr[idx] = val;
}

// check if da is empty.
// possible errors: 
// - DaOutOfRangeError
bool DaEmpty(DynamicArray *da) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return true;
    }
    DaError = DaNoError;
    return da->len == 0;
}


// distorys da, and set its pointer to NULL
// possible errors: 
// - DaIsNULLError
void DaDestroy(DynamicArray **da) {
    if (*da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    free((*da)->arr);
    free(*da);
    *da = NULL;
    DaError = DaNoError;
}
