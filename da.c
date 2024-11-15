#include "da.h"

static DaErrorType DaError = DaNoError;

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
        case DaInvalidElemSizeError: return "invalid element size"; break;
        default: return "invalid error type"; break;
    }
}

void *NewDynamicArray(size_t cap, size_t elemSize) {
    if (elemSize == 0) {
        DaError = DaInvalidElemSizeError;
        return NULL;
    }
    DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (da == NULL) {
        DaError = DaAllocFailedError;
        return NULL;
    }
    da->arr = malloc(cap * elemSize);
    if (da->arr == NULL) {
        DaError = DaAllocFailedError;
        free(da);
        return NULL;
    }
    da->cap = cap;
    da->len = 0;
    da->elemSize = elemSize;
    DaError = DaNoError;
    return da;
}

// resize the internal array of da and doubling the capacity.
// returns -1 if resize failed.
static int DaResize(DynamicArray *da) {
    if (da->cap == 0) {
        da->cap = 1;
    }
    da->arr = realloc(da->arr, da->elemSize * da->cap * 2);
    if (da->arr == NULL) {
        return -1;
    }
    da->cap *= 2;
    return 0;
}

void DaAppend(DynamicArray *da, void *val) {
    if (da == NULL || val == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len >= da->cap) {
        if (DaResize(da) == -1) {
            DaError = DaAllocFailedError;
            return;
        }
    }
    void *destPtr = (char *)da->arr + (da->elemSize * da->len);
    memcpy(destPtr, val, da->elemSize);
    da->len++;
    DaError = DaNoError;
}

void DaPop(DynamicArray *da, void *outElem) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return;
    }
    DaError = DaNoError;
    void *destPtr = (char *)da->arr + (da->elemSize * (da->len - 1));
    memcpy(outElem, destPtr, da->elemSize);
    da->len--; // TODO: might handle shrink
}

void DaFront(DynamicArray *da, void *outElem) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return;
    }
    DaError = DaNoError;
    memcpy(outElem, da->arr, da->elemSize);
}

void DaBack(DynamicArray *da, void *outElem) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return;
    }
    DaError = DaNoError;
    void *destPtr = (char *)da->arr + (da->elemSize * (da->len - 1));
    memcpy(outElem, destPtr, da->elemSize);
}

void DaGet(DynamicArray *da, size_t idx, void *outElem) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len <= idx) {
        DaError = DaOutOfRangeError;
        return;
    }
    DaError = DaNoError;
    void *destPtr = (char *)da->arr + (da->elemSize * idx);
    memcpy(outElem, destPtr, da->elemSize);
}

void DaSet(DynamicArray *da, size_t idx, void *val) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len <= idx) {
        DaError = DaOutOfRangeError;
        return;
    }
    DaError = DaNoError;
    void *destPtr = (char *)da->arr + (da->elemSize * idx);
    memcpy(destPtr, val, da->elemSize);
}

bool DaEmpty(DynamicArray *da) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return true;
    }
    DaError = DaNoError;
    return da->len == 0;
}

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
