#ifndef MH_LIB_LIST_HPP
#define MH_LIB_LIST_HPP
#include "../core/types.hpp"
#include <string.h>

#define LIST_DEFAULT_CAP 8

template <typename T>
class List {
public:
  T *data;
  u32 len;
  u32 cap;

  List() {
    this->cap = LIST_DEFAULT_CAP;
    this->len = 0;
    this->data = new T [this->cap];
  }

  List(u32 size) {
    this->cap = size;
    this->len = 0;
    this->data = new T [this->cap];
  }

  ~List() {
    delete [] this->data;
  }

#define LIST_EXPEREMENT
  void append(T element) {
#ifdef LIST_EXPEREMENT
    this->data[this->len] = element;
    this->len++;
    if (this->len == this->cap) {
      this->cap <<= 1;
      T *new_data = new T [this->cap];
      for(i32 i = 0; i < this->len; i++) {
        new_data[i] = this->data[i];
      }
      delete [] this->data;
      this->data = new_data;
    }
#else
    this->data[this->len] = element;
    incLen();
#endif

  }

  void append(T *elements, u32 size) {
    grow(size);
    memcpy(&this->data[this->len], elements, sizeof(T) * size);
    this->len += size;
  }

  //0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  void insert(T element, u32 pos) {

    T *tmp_data = new T [this->len - pos];
    memcpy(tmp_data, &this->data[pos], sizeof(T) * (this->len - pos));
    this->data[pos] = element;
    memcpy(&this->data[pos + 1], tmp_data, sizeof(T) * (this->len - pos));
    incLen();
    delete [] tmp_data;
  }


  void insert(T *elements, u32 size, u32 pos) {
    grow(size);
    T *tmp_data = new T [this->len - pos];
    memcpy(tmp_data, &this->data[pos], sizeof(T) * (this->len - pos));
    memcpy(&this->data[pos], elements, sizeof(T) * size);
    memcpy(&this->data[pos + size + 1], tmp_data, sizeof(T) * (this->len - pos));
    this->len += size;
    delete [] tmp_data;
  }

  void removeUnordered(u32 index) {
    if (index >= this->len) return;
    this->data[index] = this->data[this->len - 1];
    this->len--;
  }

  //0, 1, 2, 3, 4, 5, 6, 7, 8, 9
  void removeOrdered(u32 index) {
    T *tmp_data = new T [this->len - index - 1];
    memcpy(tmp_data, &this->data[index + 1], sizeof(T) * (this->len - index - 1));
    memcpy(&this->data[index], tmp_data, sizeof(T) * (this->len - index - 1));
    this->len--;
  }

  void clear() {
    memset(this->data, 0, this->cap * sizeof(T));
    this->len = 0;
  }

  T& operator[](u32 index) {
    if (index >= this->len) {
      return this->data[0];
    }
    return this->data[index];
  }

  bool contain(T element) {
    for(i32 i = 0; i < this->len; i++) {
      if (this->data[i] == element) {
        return true;
      }
    }
    return false;
  }

  Optional<T> get(u32 index) {
    if (index > this->len) {
      return Optional<T>();
    }
    return Optional<T>(&this->data[index]);
  }

private:
  void inline incLen() {
    this->len++;
    if (this->cap == this->len) {
      T *new_data = new T [this->cap << 1];
      memcpy(new_data, this->data, sizeof(T) * this->cap);
      this->cap <<= 1;
      delete [] this->data;
      this->data = new_data;
    }
  }

  void grow(u32 size) {
    while((this->cap - this->len) <= size) {
      T *new_data = new T [this->cap << 1];
      memcpy(new_data, this->data, sizeof(T) * this->cap);
      this->cap <<= 1;
      delete [] this->data;
      this->data = new_data;
    }
  }
};

#endif //MH_LIB_LIST_HPP
