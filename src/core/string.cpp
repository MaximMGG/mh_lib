#include "../../headers/core/string.hpp"

const u32 TRIM_SYMBOLS_LEN = 6;
const i8 *TRIM_SYMBOLS = "\a\r\n\t\b ";

static bool __streql(i8 *s, i8 *other, u32 len) {
  u32 big_cycles = len / 8;
  if (big_cycles < 1) {
    for(i32 i = 0; i < len; i++) {
      if (s[i] != other[i]) return false;
    }
  }
  u64 *tmp_s = cast(u64 *, s);
  u64 *tmp_other = cast(u64 *, other);
  while((big_cycles--) > 0) {
    if (*(tmp_s++) != *(tmp_other)) return false;
  }
  u32 small_cycles = len % 8;
  if (small_cycles < 1) {
    return true;
  }
  for(i32 i = (len / 8) * 8; i < len; i++) {
    if (s[i] != other[i]) return false;
  }
  return true;
}

static u32 __strlen(i8 *str) {
  i8 *tmp = str;
  while((*(tmp++) != '\0') && (*tmp != '\n'));
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
    for (i32 i = (len / 8) * 8; i < len; i++) {
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

void String::concat(const i8 *str, u32 len) {
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
  u32 pattern_len = __strlen((i8 *)pattern);
  if (pattern_len > this->len) {
    return false;
  }
  for(i32 i = 0; i < pattern_len; i++) {
    if (pattern[i] != this->data[i]) {
      return false;
    }
  }
  return true;
}

bool String::endsWith(const i8 *pattern) {
  u32 pattern_len = __strlen((i8 *)pattern);
  if (pattern_len >= this->len) return false;

  u32 p_i = 0;
  for(i32 i = this->len - pattern_len; i < this->len; i++) {
    if (this->data[i] != pattern[p_i]) return false;
    p_i++;
  }
  return true;
}

i32 String::indexOf(const i8 *pattern) {
  u32 p_len = __strlen((i8 *)pattern);
  if (p_len > this->len) return -1;
  for(i32 i = 0; i < this->len; i++) {
    if (pattern[0] == this->data[i]) {
      if (this->len - i < p_len) return -1;
      if (__streql(&this->data[i], (i8 *)pattern, p_len)) {
        return i;
      }
    }
  }
  return -1;
}

i32 String::indexOf(i8 c) {
  for(i32 i = 0; i < this->len; i++) {
    if (this->data[i] == c) {
      return i;
    }
  }
  return -1;
}

i32 String::indexOf(String& s) {
  if (s.len > this->len) return -1;
  for(i32 i = 0; i < this->len; i++) {
    if (s.data[0] == this->data[i]) {
      if (this->len - i < s.len) return -1;
      if (__streql(&this->data[i], (i8 *)s.data, s.len)) {
        return i;
      }
    }
  }
  return -1;
}

bool String::eql(String &other) {
  return __streql(this->data, other.data, this->len);
}

bool String::eql(const i8 *s) {
  return __streql(this->data, (i8 *)s, this->len);
}

DArr<String> String::split(const i8 *pattern) {
  return DArr<String>{};
}
  
String::~String() {
  delete [] this->data; 
}

bool String::operator==(String& b) {
  return eql(b);
}

bool String::operator==(const i8 *s) {
  return eql(s);
}

i8 String::operator[](i32 index) {
  if ((index < 0) || (index >= this->len)) return 0;
  return this->data[index];
}

void String::operator+=(String& b) {
  concat(b);
}

void String::operator+=(i8 c) {
  i8 buf[2] = {c, '\0'};
  concat(buf);
}

void String::operator+=(const i8 *s) {
  concat(s);
}

String String::operator+(String& b) {
  String n_str{this};
  n_str += b;
  return n_str;
}

String String::operator+(const i8 *s) {
  String n_str{this};
  n_str += s;
  return n_str;
}

String String::operator+(i8 c) {
  String n_str{this};
  i8 buf[2] = {c, '\0'};
  n_str += buf;
  return n_str;
}

void String::replace(const i8 *pattern, const i8 *s) {
  u32 p_len = __strlen((i8 *)pattern);
  u32 s_len = __strlen((i8 *)s);
  i32 index = 0;
  i32 pr_index = 0;
  String acum{""};
  
  while(index < this->len) {
    if (pattern[0] == this->data[index]) {
      if (__streql((i8 *)pattern, &this->data[index], p_len)) {
        acum.concat(&this->data[pr_index], index - pr_index);
        acum.concat(s, s_len);
        pr_index = index + p_len;
        index += p_len;
      } else {
        index++;
      }
    } else {
      index++;
    }
  }

  if (pr_index < index) {
    acum.concat(&this->data[pr_index], index - pr_index);
  }
  
  delete [] this->data;
  i8 *new_data = new i8 [acum.len + 1];
  __strcpy(new_data, acum.data, acum.len);
  this->data = new_data;
  this->len = acum.len;
}
