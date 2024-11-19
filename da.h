#ifndef DA_H_
#define DA_H_

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// TODO: other features
// [ ] DaSort -> takes a comparison func

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

extern DaErrorType DaGetError();
extern const char *DaGetErrMsg();

// create a new dynamic array on the heap.
// if argument cap is set to 0, the default capcity is elem_size.
// possible errors: 
// - DaIsNULLError
// - DaAllocFailedError
extern void *NewDynamicArray(size_t cap, size_t elemSize);

// append an element at the end of da.
// possible errors: 
// - DaIsNULLError
// - DaAllocFailedError
extern void DaAppend(DynamicArray *da, void *val);

// delete and copy the last element in da to outElem.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
extern void DaPop(DynamicArray *da, void *outElem);

// copy the first element in da to outElem.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
extern void DaFront(DynamicArray *da, void *outElem);

// copy the last element in da to outElem.
// possible errors: 
// - DaIsNULLError
// - DaIsEmptyError
extern void DaBack(DynamicArray *da, void *outElem);

// get a value in da at idx and copy it to outElem.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
extern void DaGet(DynamicArray *da, size_t idx, void *outElem);

// set a value with val in da at idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
extern void DaSet(DynamicArray *da, size_t idx, void *val);

// destroys the da and assign it to a new empty one.
// possible errors: 
// - DaIsNULLError
extern void DaClear(DynamicArray **da);

// reverse da content.
// possible errors: 
// - DaIsNULLError
extern void DaReverse(DynamicArray *da);

// insert an element at a specific idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
extern void DaInsertAt(DynamicArray *da, size_t idx, void *val);

// remove an element at a specific idx.
// possible errors: 
// - DaIsNULLError
// - DaOutOfRangeError
extern void DaRemoveAt(DynamicArray *da, size_t idx);

// check if da is empty.
// possible errors: 
// - DaIsNULLError
extern bool DaEmpty(DynamicArray *da);

// distorys da, and set its pointer to NULL
// possible errors: 
// - DaIsNULLError
extern void DaDestroy(DynamicArray **da);

extern void DaDeepCopy(DynamicArray *src, DynamicArray *dest);

// return a pointer to max element. 
// comp retuns the difference between a, b. if a is the first arg and b is the second, 
// it should return 0 if a == b, positive if a > b, or negative if a < b.
// possible errors:
// - DaIsNULLError
// - DaIsEmptyError
extern void *DaMax(DynamicArray *da, int (*comp)(void *, void *));

// return a pointer to min element. 
// comp retuns the difference between a, b. if a is the first arg and b is the second, 
// it should return 0 if a == b, positive if a > b, or negative if a < b.
// possible errors:
// - DaIsNULLError
// - DaIsEmptyError
extern void *DaMin(DynamicArray *da, int (*comp)(void *, void *));

// return index of the first element matches key
// possible errors:
// - DaIsNULLError
extern size_t DaIndex(DynamicArray *da, void *key);

// return index of the first element matches predicate.
// possible errors:
// - DaIsNULLError
extern size_t DaIndexFunc(DynamicArray *da, bool (*predicate)(void *));

#endif // DA_H_ 
