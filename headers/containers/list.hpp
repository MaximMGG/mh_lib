#ifndef MH_LIB_LIST_HPP
#define MH_LIB_LIST_HPP
#include "../core/types.hpp"


template <typename T>
class List {
public:
  T *data;
  u32 len;

  List();
  List(u32 size);
  ~List();

  void append(T element);
  void append(T *elements, u32 size);
  void insert(T element, u32 pos);
  void insert(T *element, u32 pos);

};



#endif //MH_LIB_LIST_HPP
