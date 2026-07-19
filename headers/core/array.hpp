#ifndef MY_ARRAY_HPP
#define MY_ARRAY_HPP
#include "types.hpp"

#define DEFAULT_DARR_CAP 8

enum SORTING_ORDER {
  DESC, ASC
};

template <typename T>
struct DArr {
  T *data;
  u32 len = 0;
  u32 cap = DEFAULT_DARR_CAP;

  DArr() {
    this->data = new T [this->cap];
    ZERO(this->data, this->cap);
  }
  DArr(T *t, u32 len);
  DArr(u32 len);
  DArr(DArr& other);
  
  ~DArr() {
    delete [] this->data;
  }

  void append(T t);
  void append(T *arr, u32 len);
  void removeOrdered(i32 index);
  void removeUnordered(i32 index);
  void contain(T t);
  void sort(SORTING_ORDER ord);

  T& operator[](i32 index);
  void operator+=(DArr& other);
  void operator+=(DArr *other);
  DArr operator+(DArr& other);
  DArr operator+(DArr *other);
};


#endif //MY_ARRAY_HPP
