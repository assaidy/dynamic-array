#ifndef DA_H_
#define DA_H_

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    size_t cap;
    size_t len;
    size_t elemSize;
    void *arr;
} DynamicArray;

typedef enum {
    DaNoError,
    DaInvalidElemSizeError,
    DaIsEmptyError,
    DaIsNULLError,
    DaAllocFailedError,
    DaOutOfRangeError,
} DaErrorType;

DaErrorType DaGetError();
const char *DaGetErrMsg();
// create a new dynamic array on the heap.
// if argument cap is set to 0, the default capcity is elem_size.
// possible errors: 
// - DaIsNULLError
// - DaAllocFailedError
DynamicArray *NewDynamicArray(size_t cap, size_t elemSize);
// append an element at the end of da.
// possible errors: 
// - DaIsNULLError
// - DaAllocFailedError
void DaAppend(DynamicArray *da, void *val);
// delete and copy the last element in da to outElem.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
void DaPop(DynamicArray *da, void *outElem);
// copy the first element in da to outElem.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
void DaFront(DynamicArray *da, void *outElem);
// copy the last element in da to outElem.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
void DaBack(DynamicArray *da, void *outElem);
// get a value in da at idx and copy it to outElem.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
void DaGet(DynamicArray *da, size_t idx, void *outElem);
// set a value with val in da at idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
void DaSet(DynamicArray *da, size_t idx, void *val);
// destroys the da and assign it to a new empty one.
// possible errors: 
// - DaIsNULLError
void DaClear(DynamicArray **da);
// reverse da content.
// possible errors: 
// - DaIsNULLError
void DaReverse(DynamicArray *da);
// insert an element at a specific idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
void DaInsertAt(DynamicArray *da, size_t idx, void *val);
// remove an element at a specific idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
void DaRemoveAt(DynamicArray *da, size_t idx);
// check if da is empty.
// possible errors: 
// - DaIsNULLError
bool DaEmpty(DynamicArray *da);
// distorys da, and set its pointer to NULL
// possible errors: 
// - DaIsNULLError
void DaDestroy(DynamicArray **da);
void DaDeepCopy(DynamicArray *src, DynamicArray *dest);
// return a pointer to max element. 
// comp retuns the difference between a, b. if a is the first arg and b is the second, 
// it should return 0 if a == b, positive if a > b, or negative if a < b.
// possible errors:
// - DaIsNULLError
// - DaIsEmptyError
void *DaMax(DynamicArray *da, int (*comp)(void *, void *));
// return a pointer to min element. 
// comp retuns the difference between a, b. if a is the first arg and b is the second, 
// it should return 0 if a == b, positive if a > b, or negative if a < b.
// possible errors:
// - DaIsNULLError
// - DaIsEmptyError
void *DaMin(DynamicArray *da, int (*comp)(void *, void *));
// return index of the first element matches key
// possible errors:
// - DaIsNULLError
size_t DaIndex(DynamicArray *da, void *key);
// return index of the first element matches predicate.
// possible errors:
// - DaIsNULLError
size_t DaIndexFunc(DynamicArray *da, bool (*predicate)(void *));

#ifdef DA_IMPLEMENTATION

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

DynamicArray *NewDynamicArray(size_t cap, size_t elemSize) {
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

#endif // DA_IMPLEMENTATION

#endif // DA_H_ 
