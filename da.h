#ifndef DA_IMPL
#define DA_IMPL

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// TODO: other features
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

// check if da is empty.
// possible errors: 
// - DaIsNULLError
extern bool DaEmpty(DynamicArray *da);

// distorys da, and set its pointer to NULL
// possible errors: 
// - DaIsNULLError
extern void DaDestroy(DynamicArray **da);

#endif // DA_IMPL
