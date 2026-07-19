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
  void concat(const i8 *str, u32 len);
  String substring(u32 from, u32 to);
  void trim();
  bool startsWith(const i8 *pattern);
  bool endsWith(const i8 *pattern);
  i32 indexOf(const i8 *pattern);
  i32 indexOf(i8 c);
  i32 indexOf(String& s);
  i32 lastIndexOf(const i8 *pattern);
  i32 lastIndexOf(i8 c);
  i32 lastIndexOf(String &s);
  bool eql(String &other);
  bool eql(const i8 *s);
  void replace(const i8 *pattern, const i8 *s);

  DArr<String> split(const i8 *pattern);

  bool operator==(String& b);
  bool operator==(const i8 *s);
  i8 operator[](i32 index);
  void operator+=(String& b);
  void operator+=(const i8 *s);
  void operator+=(i8 c);

  String operator+(String& b);
  String operator+(const i8 *s);
  String operator+(i8 c);
  
  ~String();

};


#endif //MH_STRING_HPP
