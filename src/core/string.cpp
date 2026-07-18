#include "../../headers/core/string.hpp"

const u32 TRIM_SYMBOLS_LEN = 4;
const i8 *TRIM_SYMBOLS = "\r\n\t\b";


static u32 __strlen(i8 *str) {
  i8 *tmp = str;
  while((*(tmp++) != '\0') || (*tmp != '\n'));
  return tmp - str - 1;
}

static void __strcpy(i8 *dest, i8 *src, u32 len) {
  u32 big_cycles = len / 8;
  if (big_cycles < 1) {
    for(i32 i = 0; i < len; i++) {
      dest[i] = src[i];
    }
    dest[len] = '\0';
  }
  
  u32 small_cycles = len % 8;
  u64 *tmp_dest = reinterpret_cast<u64 *>(dest);
  u64 *tmp_src = cast(u64 *, src);
  while((big_cycles--) > 0) *(tmp_dest++) = *(tmp_src++);
  if (small_cycles == 0) {
    dest[len] = '\0';
    return;
  } else {
    for (i32 i = (len % 8) * 8; i < len; i++) {
      dest[i] = src[i];
    }
    dest[len] = '\0';
  }
}


String::String(const i8 *str) {
  this->len = __strlen((i8 *)str);
  this->data = new i8[len + 1];
  __strcpy(this->data, (i8 *)str, len);
}

String::String(i8 *str) {
  this->len = __strlen(str);
  this->data = new i8[len + 1];
  __strcpy(this->data, (i8 *)str, len);
}

String::String(String *s) {
  this->len = s->len;
  this->data = new i8 [s->len + 1];
  __strcpy(this->data, s->data, s->len);
}

String::String(String& s) {
  this->len = s.len;
  this->data = new i8 [s.len + 1];
  __strcpy(this->data, s.data, s.len);
}

String::String(const String& s) {
  this->len = s.len;
  this->data = new i8 [s.len + 1];
  __strcpy(this->data, s.data, s.len);
}

String::String(i8 *str, u32 len) {
  this->len = len;
  this->data = new i8 [len + 1];
  __strcpy(this->data, str, len);
}

void String::concat(String &s) {
  u32 new_len = this->len + s.len;
  i8 *new_data = new i8 [new_len + 1];
  __strcpy(new_data, this->data, this->len);
  __strcpy(&new_data[this->len], s.data, s.len);
  delete [] this->data;
  this->data = new_data;
  this->len = new_len;
}

void String::concat(const i8 *str) {
  u32 len = __strlen((i8 *)str);
  u32 new_len = this->len + len;
  i8 *new_data = new i8 [new_len + 1];
  __strcpy(new_data, this->data, this->len);
  __strcpy(&new_data[this->len], (i8 *)str, len);
  delete [] this->data;
  this->data = new_data;
  this->len = new_len;
}

String String::substring(u32 from, u32 to) {
  if ((from < 0) || (to >= this->len) || (from > to)) {
    return String{};
  }
  
  return String{&this->data[from], to - from};
}

bool isTrimSymbol(i8 c) {
  for(i32 i = 0; i < TRIM_SYMBOLS_LEN; i++) {
    if (c == TRIM_SYMBOLS[i]) {
      return true;
    }
  }
  return false;
}

void String::trim() {
  i32 buf_i = 0;
  for(i32 i = 0; i < this->len; i++) {
    if (isTrimSymbol(this->data[i])) {
      buf_i++;
    } else {
      break;
    }
  }
  i32 end_i = 0;
  for(i32 i = this->len - 1; i > 0; i++) {
    if (isTrimSymbol(this->data[i])) {
      end_i++;
    } else {
      break;
    }
  }
  u32 new_len = this->len - (buf_i + end_i);
  i8 *new_data = new i8 [this->len - (new_len + 1)];
  __strcpy(new_data, &this->data[buf_i], new_len);
  delete [] this->data;
  this->data = new_data;
  this->len = new_len;
}

bool String::startsWith(const i8 *pattern) {
  return false;
}
bool String::endsWith(const i8 *pattern) {
  return false;
}
i32 String::indexOf(const i8 *pattern) {
  return false;
}
i32 String::indexOf(i8 c) {
  return false;
}
i32 String::indexOf(String& s) {
  return -1;
}
DArr<String> String::split(const i8 *pattern) {
  return DArr<String>{};
}
  
String::~String() {
  delete [] this->data; 
}
