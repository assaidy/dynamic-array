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
    daNoError,              // no error in the last operation
    daInvalidElemSizeError, // element size should be > 0
    daIsEmptyError,         // da->arr is empty
    daIsNULLError,          // da or da->arr is NULL
    daAllocFailedError,     // failed to alloc/realloc da->arr
    daOutOfRangeError,      // index out of da->arr's range
} daErrorType;

static daErrorType daError = daNoError;

daErrorType daGetError();
// initialize a dynamic array.
// if argument cap is set to 0, the default capcity is elemSize.
// possible errors: 
// - daIsNULLError
// - daAllocFailedError
void daInit(DynamicArray *da, size_t cap, size_t elemSize);
// append an element at the end of da.
// possible errors: 
// - daIsNULLError
// - daAllocFailedError
void daAppend(DynamicArray *da, void *val);
// delete and return the last element.
// possible errors: 
// - daIsNULLError
// - daIsEmptyError
void *daPop(DynamicArray *da);
// return the first element.
// possible errors: 
// - daIsNULLError
// - daIsEmptyError
void *daFront(DynamicArray *da);
// return the last element.
// possible errors: 
// - daIsNULLError
// - daIsEmptyError
void *daBack(DynamicArray *da);
// return a value in da at idx.
// possible errors: 
// - daIsNULLError
// - daOutOfRangeError
void *daGet(DynamicArray *da, size_t idx);
// set a value with val in da at idx.
// possible errors: 
// - daIsNULLError
// - daOutOfRangeError
void daSet(DynamicArray *da, size_t idx, void *val);
// reset the da, and free allocated memory.
// possible errors: 
// - daIsNULLError
void daClear(DynamicArray *da);
// reverse da content.
// possible errors: 
// - daIsNULLError
void daReverse(DynamicArray *da);
// insert an element at a specific idx.
// possible errors: 
// - daIsNULLError
// - daOutOfRangeError
void daInsertAt(DynamicArray *da, size_t idx, void *val);
// remove an element at a specific idx.
// possible errors: 
// - daIsNULLError
// - daOutOfRangeError
void daRemoveAt(DynamicArray *da, size_t idx);
// check if da is empty.
// possible errors: 
// - daIsNULLError
bool daEmpty(DynamicArray *da);
// free allocated memory for da->arr.
// possible errors: 
// - daIsNULLError
void daDestroy(DynamicArray *da);
void daDeepCopy(DynamicArray *src, DynamicArray *dest);
// return a pointer to max element. 
// comp retuns the difference between a, b. if a is the first arg and b is the second, 
// it should return 0 if a == b, positive if a > b, or negative if a < b.
// possible errors:
// - daIsNULLError
// - daIsEmptyError
void *daMax(DynamicArray *da, int (*comp)(void *, void *));
// return a pointer to min element. 
// comp retuns the difference between a, b. if a is the first arg and b is the second, 
// it should return 0 if a == b, positive if a > b, or negative if a < b.
// possible errors:
// - daIsNULLError
// - daIsEmptyError
void *daMin(DynamicArray *da, int (*comp)(void *, void *));
// return index of the first element matches key
// possible errors:
// - daIsNULLError
size_t daIndex(DynamicArray *da, void *key);
// return index of the first element matches predicate.
// possible errors:
// - daIsNULLError
size_t daIndexFunc(DynamicArray *da, bool (*predicate)(void *));

#ifdef DA_IMPLEMENTATION

daErrorType daGetError() {
    return daError;
}

void daInit(DynamicArray *da, size_t cap, size_t elemSize) {
    if (elemSize == 0) {
        daError = daInvalidElemSizeError;
        return;
    }
    if (da == NULL) {
        daError = daAllocFailedError;
        return;
    }
    da->arr = malloc(cap * elemSize);
    if (da->arr == NULL) {
        daError = daAllocFailedError;
        return;
    }
    da->cap = cap;
    da->len = 0;
    da->elemSize = elemSize;
    daError = daNoError;
}

// resize the internal array of da and double the capacity.
// returns -1 if resize failed.
static int daResize(DynamicArray *da) {
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

void daAppend(DynamicArray *da, void *val) {
    if (da == NULL || val == NULL) {
        daError = daIsNULLError;
        return;
    }
    if (da->len >= da->cap) {
        if (daResize(da) == -1) {
            daError = daAllocFailedError;
            return;
        }
    }
    void *destPtr = (char *)da->arr + (da->elemSize * da->len);
    memcpy(destPtr, val, da->elemSize);
    da->len++;
    daError = daNoError;
}

void *daPop(DynamicArray *da) {
    if (da == NULL) {
        daError = daIsNULLError;
        return NULL;
    }
    if (da->len == 0) {
        daError = daIsEmptyError;
        return NULL;
    }
    daError = daNoError;
    da->len--; // TODO: might handle shrink
    void *destPtr = (char *)da->arr + (da->elemSize * (da->len - 1));
    return destPtr;
}

void *daFront(DynamicArray *da) {
    if (da == NULL) {
        daError = daIsNULLError;
        return NULL;
    }
    if (da->len == 0) {
        daError = daIsEmptyError;
        return NULL;
    }
    daError = daNoError;
    return da->arr;
}

void *daBack(DynamicArray *da) {
    if (da == NULL) {
        daError = daIsNULLError;
        return NULL;
    }
    if (da->len == 0) {
        daError = daIsEmptyError;
        return NULL;
    }
    daError = daNoError;
    void *destPtr = (char *)da->arr + (da->elemSize * (da->len - 1));
    return destPtr;
}

void *daGet(DynamicArray *da, size_t idx) {
    if (da == NULL) {
        daError = daIsNULLError;
        return NULL;
    }
    if (da->len <= idx) {
        daError = daOutOfRangeError;
        return NULL;
    }
    daError = daNoError;
    void *destPtr = (char *)da->arr + (da->elemSize * idx);
    return destPtr;
}

void daSet(DynamicArray *da, size_t idx, void *val) {
    if (da == NULL) {
        daError = daIsNULLError;
        return;
    }
    if (da->len <= idx) {
        daError = daOutOfRangeError;
        return;
    }
    daError = daNoError;
    void *destPtr = (char *)da->arr + (da->elemSize * idx);
    memcpy(destPtr, val, da->elemSize);
}

void daClear(DynamicArray *da) {
    if (da == NULL) {
        daError = daIsNULLError;
        return;
    }
    daError = daNoError;
    size_t elemSize = da->elemSize;
    daDestroy(da);
    daInit(da, 0, elemSize);
}

static void daSwap(void *a, void *b, size_t elemSize) {
    void *tmp = malloc(elemSize);
    memcpy(tmp, a, elemSize);
    memcpy(a, b, elemSize);
    memcpy(b, tmp, elemSize);
    free(tmp);
}

void daReverse(DynamicArray *da) {
    if (da == NULL) {
        daError = daIsNULLError;
        return;
    }
    if (da->len == 0) {
        return;
    }
    daError = daNoError;
    for (size_t i = 0; i < da->len / 2; i++) {
        daSwap(
            (char *)da->arr + (da->elemSize * i),
            (char *)da->arr + (da->elemSize * (da->len - i - 1)),
            da->elemSize
        );
    }
}

static void daShiftRight(DynamicArray *da, size_t idx) {
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

void daInsertAt(DynamicArray *da, size_t idx, void *val) {
    if (da == NULL) {
        daError = daIsNULLError;
        return;
    }
    if (da->len <= idx) {
        daError = daOutOfRangeError;
        return;
    }
    if (da->len >= da->cap) {
        if (daResize(da) == -1) {
            daError = daAllocFailedError;
            return;
        }
    }
    daError = daNoError;
    daShiftRight(da, idx);
    void *ptr = (char *)da->arr + (da->elemSize * idx);
    memcpy(ptr, val, da->elemSize);
    da->len++;
}

void daRemoveAt(DynamicArray *da, size_t idx) {
    if (da == NULL) {
        daError = daIsNULLError;
        return;
    }
    if (da->len <= idx) {
        daError = daOutOfRangeError;
        return;
    }
    daError = daNoError;
    DsShiftLeft(da, idx);
    da->len--;
}

bool daEmpty(DynamicArray *da) {
    if (da == NULL) {
        daError = daIsNULLError;
        return true;
    }
    daError = daNoError;
    return da->len == 0;
}

void daDestroy(DynamicArray *da) {
    if (da == NULL) {
        daError = daIsNULLError;
        return;
    }
    daError = daNoError;
    free(da->arr);
    da->arr = NULL;
}

void daDeepCopy(DynamicArray *src, DynamicArray *dest) {
    if (src == NULL || dest == NULL) {
        daError = daIsNULLError;
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
            daError = daAllocFailedError;
            return;
        }
        dest->arr = newArr;
        memcpy(dest->arr, src->arr, src->len * src->elemSize);
    }
    dest->len = src->len;
    dest->cap = src->cap;
    dest->elemSize = src->elemSize;
    daError = daNoError;
}

void *daMax(DynamicArray *da, int (*comp)(void *, void *)) {
    if (da == NULL || da->arr == NULL) {
        daError = daIsNULLError;
        return NULL;
    }
    if (da->len == 0) {
        daError = daIsEmptyError;
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

void *daMin(DynamicArray *da, int (*comp)(void *, void *)) {
    if (da == NULL || da->arr == NULL) {
        daError = daIsNULLError;
        return NULL;
    }
    if (da->len == 0) {
        daError = daIsEmptyError;
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

size_t daIndex(DynamicArray *da, void *key) {
    if (da == NULL || da->arr == NULL) {
        daError = daIsNULLError;
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

size_t daIndexFunc(DynamicArray *da, bool (*predicate)(void *)) {
    if (da == NULL || da->arr == NULL) {
        daError = daIsNULLError;
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
