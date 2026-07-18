#ifndef MH_STRING_HPP
#define MH_STRING_HPP
#include "types.hpp"
#include "array.hpp"

struct String {
  i8 *data;
  u32 len;

  String() = default;
  String(const i8 *str);
  String(i8 *str);
  String(String *s);
  String(String& s);
  String(const String& s);
  String(i8 *str, u32 len);

  void concat(String &s);
  void concat(const i8 *str);
  String substring(u32 from, u32 to);
  void trim();
  bool startsWith(const i8 *pattern);
  bool endsWith(const i8 *pattern);
  i32 indexOf(const i8 *pattern);
  i32 indexOf(i8 c);
  i32 indexOf(String& s);
  DArr<String> split(const i8 *pattern);
  
  ~String();

};


#endif //MH_STRING_HPP
