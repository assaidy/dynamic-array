#include "da.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// resize the internal array of da and double the capacity.
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

void DaClear(DynamicArray **da) {
    if (*da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    DaError = DaNoError;
    size_t elemSize = (*da)->elemSize;
    DaDestroy(da);
    *da = NewDynamicArray(0, elemSize);
}

static void DaSwap(void *a, void *b, size_t elemSize) {
    void *tmp = malloc(elemSize);
    memcpy(tmp, a, elemSize);
    memcpy(a, b, elemSize);
    memcpy(b, tmp, elemSize);
    free(tmp);
}

void DaReverse(DynamicArray *da) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len == 0) {
        return;
    }
    DaError = DaNoError;
    for (size_t i = 0; i < da->len / 2; i++) {
        DaSwap(
            (char *)da->arr + (da->elemSize * i),
            (char *)da->arr + (da->elemSize * (da->len - i - 1)),
            da->elemSize
        );
    }
}

static void DaShiftRight(DynamicArray *da, size_t idx) {
    for (size_t i = da->len; i > idx; i--) {
        memcpy(
            (char *)da->arr + (da->elemSize * i),
            (char *)da->arr + (da->elemSize * (i - 1)),
            da->elemSize
        );
    }
}

static void DsShiftLeft(DynamicArray *da, size_t idx) {
    if (idx == 0 && da->len == 1) {
        return; // just len-- in the caller function is enough
    }
    for (size_t i = idx; i < da->len - 1; i++) {
        memcpy(
            (char *)da->arr + (da->elemSize * i),
            (char *)da->arr + (da->elemSize * (i + 1)),
            da->elemSize
        );
    }
}

void DaInsertAt(DynamicArray *da, size_t idx, void *val) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len <= idx) {
        DaError = DaOutOfRangeError;
        return;
    }
    if (da->len >= da->cap) {
        if (DaResize(da) == -1) {
            DaError = DaAllocFailedError;
            return;
        }
    }
    DaError = DaNoError;
    DaShiftRight(da, idx);
    void *ptr = (char *)da->arr + (da->elemSize * idx);
    memcpy(ptr, val, da->elemSize);
    da->len++;
}

void DaRemoveAt(DynamicArray *da, size_t idx) {
    if (da == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (da->len <= idx) {
        DaError = DaOutOfRangeError;
        return;
    }
    DaError = DaNoError;
    DsShiftLeft(da, idx);
    da->len--;
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

void DaDeepCopy(DynamicArray *src, DynamicArray *dest) {
    if (src == NULL || dest == NULL) {
        DaError = DaIsNULLError;
        return;
    }
    if (src->arr == NULL) {
        if (dest->arr != NULL) {
            free(dest->arr);
        }
        dest->arr = NULL;
    } else {
        void *newArr = (dest->arr == NULL)
            ? malloc(src->cap * src->elemSize)
            : realloc(dest->arr, src->cap * src->elemSize);
        if (newArr == NULL) {
            DaError = DaAllocFailedError;
            return;
        }
        dest->arr = newArr;
        memcpy(dest->arr, src->arr, src->len * src->elemSize);
    }
    dest->len = src->len;
    dest->cap = src->cap;
    dest->elemSize = src->elemSize;
    DaError = DaNoError;
}

void *DaMax(DynamicArray *da, int (*comp)(void *, void *)) {
    if (da == NULL || da->arr == NULL) {
        DaError = DaIsNULLError;
        return NULL;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return NULL;
    }
    void *max = da->arr;
    for (size_t i = 1; i < da->len; i++) {
        void *cur = (char *)da->arr + (da->elemSize * i);
        if (comp(cur, max) > 0) {
            max = cur;
        } 
    }
    return max;
}

void *DaMin(DynamicArray *da, int (*comp)(void *, void *)) {
    if (da == NULL || da->arr == NULL) {
        DaError = DaIsNULLError;
        return NULL;
    }
    if (da->len == 0) {
        DaError = DaIsEmptyError;
        return NULL;
    }
    void *min = da->arr;
    for (size_t i = 1; i < da->len; i++) {
        void *cur = (char *)da->arr + (da->elemSize * i);
        if (comp(cur, min) < 0) {
            min = cur;
        } 
    }
    return min;
}

size_t DaIndex(DynamicArray *da, void *key) {
    if (da == NULL || da->arr == NULL) {
        DaError = DaIsNULLError;
        return -1;
    }
    if (da->len == 0) {
        return -1;
    }
    size_t idx = -1;
    for (int i = 0; i < da->len; i++) {
        void *cur = (char *)da->arr + (da->elemSize * i);
        if (memcmp(key, cur, da->elemSize) == 0) {
            idx = i;
            break;
        }
    }
    return idx;
}

size_t DaIndexFunc(DynamicArray *da, bool (*predicate)(void *)) {
    if (da == NULL || da->arr == NULL) {
        DaError = DaIsNULLError;
        return -1;
    }
    if (da->len == 0) {
        return -1;
    }
    size_t idx = -1;
    for (int i = 0; i < da->len; i++) {
        void *cur = (char *)da->arr + (da->elemSize * i);
        if (predicate(cur)) {
            idx = i;
            break;
        }
    }
    return idx;
}
