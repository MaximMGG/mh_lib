#ifndef MY_ARRAY_HPP
#define MY_ARRAY_HPP
#include "types.hpp"
#include <string.h>

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
  DArr(T *t, u32 len) {
    while(len > this->cap) {
      this->cap <<= 1;
    }
    this->data = new T [this->cap];
    ZERO(this->data, this->cap);
    memcpy(this->data, t, sizeof(T) * len);
    this->len = len;
  }
  
  DArr(u32 len) {
    while(len > this->cap) {
      this->cap <<= 1;
    }
    this->data = new T [this->cap];
    ZERO(this->data, this->cap);
  }
  
  DArr(DArr& other) {
    this->cap = other.cap;
    this->data = new T [this->cap];
    ZERO(this->data, this->cap);
    memcpy(this->data, other.data, sizeof(T) * other.len);
    this->len = other.len;
  }

  DArr(DArr *other) {
    this->cap = other->cap;
    this->data = new T [this->cap];
    ZERO(this->data, this->cap);
    memcpy(this->data, other->data, sizeof(T) * other->len);
    this->len = other->len;
  }
  
  ~DArr() {
    delete [] this->data;
  }

  void append(T t) {
    if (this->len == this->cap) {
      T *new_data = new T[this->cap << 1];
      ZERO(new_data, this->cap << 1);
      memcpy(new_data, this->data, sizeof(T) * this->cap);
      this->cap <<= 1;
      delete [] this->data;
      this->data = new_data;
    }
    this->data[this->len++] = t;
  }
  
  void append(T *arr, u32 len) {
    while(this->len + len > this->cap) {
      this->cap <<= 1;
      T *new_data = new T [this->cap];
      ZERO(new_data, this->cap);
      memcpy(new_data, this->data, sizeof(T) * this->len);
      delete [] this->data;
      this->data = new_data;
    }
    for(i32 i = 0; i < len; i++) {
      this->data[this->len++] = arr[i];
    }
  }
  //0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  //            ^
  void removeOrdered(i32 index) {
    if (index < 0 || index >= this->len) {
      return;
    }
    if (index == this->len - 1) {
      this->len--;
      return;
    }

    T *tmp_arr = new T [this->len - index - 1];
    memcpy(tmp_arr, &this->data[index + 1], sizeof(T) * (this->len - index - 1));
    memcpy(&this->data[index], tmp_arr, sizeof(T) * (this->len - index - 1));
    this->len--;
  }
  
  void removeUnordered(i32 index) {
    if (index < 0 || index >= this->len) {
      return;
    }
    this->data[index] = this->data[this->len - 1];
    this->len--;
  }
  
  bool contain(T t) {
    for(i32 i = 0; i < this->len; i++) {
      if (this->data[i] == t) {
	return true;
      }
    }
    return false;
  }


  void sort(SORTING_ORDER ord);


  //if index < 0 || index >= this->len it will olways return [0] element
  T& operator[](i32 index) {
    if (index < 0 || index >= this->len) {
      return this->data[0];
    }
    return this->data[index];
  }
  
  void operator+=(DArr& other) {
    T *new_data = new T [this->cap + other.cap];
    memcpy(new_data, this->data, sizeof(T) * this->len);
    memcpy(&new_data[this->len], other.data, sizeof(T) * other.len);
    delete [] this->data;
    this->data = new_data;
    this->len += other.len;
    this->cap += other.cap;
  }
  
  void operator+=(DArr *other) {
    T *new_data = new T [this->cap + other->cap];
    memcpy(new_data, this->data, sizeof(T) * this->len);
    memcpy(&new_data[this->len], other->data, sizeof(T) * other->len);
    delete [] this->data;
    this->data = new_data;
    this->len += other->len;
    this->cap += other->cap;
  }
  
  DArr operator+(DArr& other) {
    this += other;
    return DArr<T>(this);
  }
  
  DArr operator+(DArr *other) {
    this += other;
    return DArr<T>(this);
  }
};


#endif //MY_ARRAY_HPP
