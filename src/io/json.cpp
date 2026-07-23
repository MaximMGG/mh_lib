#include "../../headers/io/json.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_OBJ_CAP 8
#define DEFAULT_ARR_CAP 8

void parseObj(Json *obj, i8 *json_src, u64 *i, u64 json_size);

bool parseBoolean(i8 *json_src, u64 *i, u64 json_size) {
  if (*i + 5 == json_size) {
    fprintf(stderr, "Incorrect Json, broken boolean\n");
  }
  if (json_src[*i] == 't') {
    if (strncmp(&json_src[*i], "true", 4) != 0) {
      fprintf(stderr, "Incorrect boolean statment\n");
      return false;
    }
    return true;
  } else if (json_src[*i] == 'f') {
    if (strncmp(&json_src[*i], "false", 5) != 0) {
      fprintf(stderr, "Incorrect boolean statment\n");
      return false;
    }
    return false;
  }
  return false;
}

String *parseString(i8 *json_src, u64 *i, u64 json_size) {
  u32 max_len = 10240;
  i8 *buf = new i8 [max_len];
  ZERO(buf, max_len);
  u32 buf_i = 0;
  
  while(json_src[*i] != '"') {
    if (*i == json_size) {
      fprintf(stderr, "Incorrect Json, broken string\n");
      delete [] buf;
    }
    if (buf_i == max_len) {
      fprintf(stderr, "Maximum length of string owerflowed\n");
      return nullptr;
    }
    buf[buf_i++] = json_src[*i];
    *i += 1;
  }
  *i += 1;
  String *res = new String{buf};
  delete [] buf;
  return res;
}

f64 parseNumber(i8 *json_src, u64 *i, u64 json_size) {
  i8 buf[128]{0};
  i32 buf_i = 0;

  bool has_dot = false;
  while(true) {
    if (*i == json_size) {
      fprintf(stderr, "Incorrect Json, broken number\n");
      return 0.0;      
    }
    if (buf_i > 128) {
      fprintf(stderr, "Maximum lenth of number owerflowed");
      return 0.0;
    }
    if (json_src[*i] == '-') {
      if (buf_i == 0) {
        buf[buf_i++] = json_src[*i];
        *i += 1;
        continue;
      } else {
        fprintf(stderr, "Incorrect Number!\n");
        return 0.0;
      }
    } else if (json_src[*i] == '.') {
      if ((buf_i > 0) && !has_dot) {
        buf[buf_i++] = json_src[*i];
        *i += 1;
        continue;
      } else {
        fprintf(stderr, "Incorrect Number!\n");
        return 0.0;
      }
    } else if (json_src[*i] >= '0' && json_src[*i] <= '9') {
      buf[buf_i++] = json_src[*i];
      *i += 1;
    } else {
      break;
    }
  }
  
  f64 res = atof(buf);
  return res;
}

Json *parseArray(Json *arr, i8 *json_src, u64 *i, u64 json_size) {
  bool type_not_defined = true;
  i32 index;
  Json *cur = new Json;
  while(true) {
    if (*i == json_size) {
      fprintf(stderr, "Incorrect Json, broken array\n");
      delete cur;
      delete arr;
      return nullptr;      
    }
    switch(json_src[*i]) {
    case '"': {
      if (type_not_defined) {
        arr->arr_type = JSON_STRING;
        type_not_defined = false;
      } else {
        if (arr->arr_type != JSON_STRING) {
          fprintf(stderr, "Different types in Json Array\n");
          delete cur;
          delete arr;
          return nullptr;
        }
      }
      *i += 1;
      cur->type = JSON_STRING;
      cur->val.j_string = parseString(json_src, i, json_size);
      arr->addString(cur);
      cur = new Json;
      if (json_src[*i] != ',') {
        break;
      }
    } break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      if (type_not_defined) {
        arr->arr_type = JSON_NUMBER;
        type_not_defined = false;
      } else {
        if (arr->arr_type != JSON_NUMBER) {
          fprintf(stderr, "Different type in Json Array\n");
          delete cur;
          delete arr;
          return nullptr;
        }
      }
      cur->type = JSON_NUMBER;
      cur->val.j_number = parseNumber(json_src, i, json_size);
      cur = new Json;
      arr->addNumber(cur);
      
      if (json_src[*i] != ',') {
        break;
      }
    } break;
    case 'f':
    case 't': {
      if (type_not_defined) {
        arr->arr_type = JSON_BOOLEAN;
        type_not_defined = false;
      } else {
        if (arr->arr_type != JSON_BOOLEAN) {
          fprintf(stderr, "Different type in Json Array\n");
          delete cur;
          delete arr;
          return nullptr;
        }
      }
      cur->type = JSON_BOOLEAN;
      cur->val.j_boolean = parseBoolean(json_src, i, json_size);
      arr->addBoolean(cur);
      cur = new Json;
      if (json_src[*i] != ',') {
        break;
      }
    } break;
    case 'n': {
      if (type_not_defined) {
        arr->arr_type = JSON_NULL;
        type_not_defined = false;
      } else {
        if (arr->arr_type != JSON_NULL) {
          fprintf(stderr, "Different type in Json Array\n");
          delete cur;
          delete arr;
          return nullptr;
        }
      }
      cur->type = JSON_NULL;
      arr->addNull(cur);
      if (json_src[*i] != ',') {
        break;
      }
      cur = new Json;
    } break;
    case '[': {
      if (type_not_defined) {
        arr->arr_type = JSON_ARRAY;
        type_not_defined = false;
      } else {
        if (arr->arr_type != JSON_ARRAY) {
          fprintf(stderr, "Different type in Json Array\n");
          delete cur;
          delete arr;
          return nullptr;
        }
      }
      *i += 1;
      Json *tmp_arr;
      tmp_arr->type = JSON_ARRAY;
      tmp_arr->root = false;
      tmp_arr->key = nullptr;

      tmp_arr->arr_len = 0;
      tmp_arr->arr_cap = DEFAULT_OBJ_CAP;
      tmp_arr->val.j_array = new Json *[tmp_arr->arr_cap];
      
      parseArray(tmp_arr, json_src, i, json_size);
      arr->addArray(tmp_arr);
      if (json_src[*i] != ',') {
        break;
      }
    } break;
    case '{': {
      if (type_not_defined) {
        arr->arr_type = JSON_OBJECT;
        type_not_defined = false;
      } else {
        if (arr->arr_type != JSON_OBJECT) {
          fprintf(stderr, "Different type in Json Array\n");
          delete cur;
          delete arr;
          return nullptr;
        }
      }
      *i += 1;
      Json *tmp_obj;
      tmp_obj->type = JSON_OBJECT;
      tmp_obj->root = false;
      tmp_obj->key = nullptr;

      tmp_obj->obj_len = 0;
      tmp_obj->obj_cap = DEFAULT_OBJ_CAP;
      tmp_obj->val.j_obj = new Json *[tmp_obj->obj_cap];
      
      parseObj(tmp_obj, json_src, i, json_size);
      arr->addObj(tmp_obj);
      if (json_src[*i] != ',') {
        break;
      }
    } break;
    default: *i += 1;
    }
    
  }
  *i += 1;
  delete cur;
  return arr;
}

void parseObj(Json *obj, i8 *json_src, u64 *i, u64 json_size) {
  bool is_key = true;

  Json *cur = new Json;
  String *tmp_key;
  while(true) {
    if (*i == json_size) {
      fprintf(stderr, "Incorrect Json, broken object\n");
      delete cur;
      delete obj;
      return;
    }
    switch(json_src[*i]) {
    case '"': {
      if (is_key) {
        *i += 1;
        tmp_key = parseString(json_src, i, json_size);
        is_key = false;
        if (json_src[*i] != ':') {
          fprintf(stderr, "Incorrect JSON after \"key\" should be ':'\n");
          delete cur;
          delete obj;
          return;
        }
        *i += 1;
      } else {
        *i += 1;
        String *val = parseString(json_src, i, json_size);
        cur->type = JSON_STRING;
        cur->key = tmp_key;
        cur->val.j_string = val;
        is_key = true;
        obj->addString(cur);
        cur = new Json;
        ZERO(cur, 1);
        if (json_src[*i] != ',') {
          delete cur;
          *i += 1;
          return;
        }
        *i += 1;
      }
    } break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      cur->type = JSON_NUMBER;
      cur->val.j_number = parseNumber(json_src, i, json_size);
      cur->key = tmp_key;
      obj->addNumber(cur);
      cur = new Json;
      ZERO(cur, 1);
      is_key = true;
      if (json_src[*i] != ',') {
        delete cur;
        *i += 1;
        return;
      }
      *i += 1;
    } break;
    case 'f':
    case 't': {
      cur->type = JSON_BOOLEAN;
      cur->val.j_boolean = parseBoolean(json_src, i, json_size);
      cur->key = tmp_key;
      obj->addBoolean(cur);
      cur = new Json;
      ZERO(cur, 1);
      is_key = true;
      if (json_src[*i] != ',') {
        delete cur;
        *i += 1;
        return;
      }
      *i += 1;
    } break;
    case 'n': {
      cur->type = JSON_NULL;
      cur->val.j_null = nullptr;
      cur->key = tmp_key;
      obj->addNull(cur);
      cur = new Json;
      ZERO(cur, 1);
      is_key = true;
      *i += 4;
      if (json_src[*i] != ',') {
        delete cur;
        *i += 1;
        return;
      }
      *i += 1;
    } break;
    case '[': {
      *i += 1;
      Json *tmp_arr = new Json{JSON_ARRAY, tmp_key->data};
      tmp_arr->type = JSON_ARRAY;
      tmp_arr->root = false;
      tmp_arr->key=  nullptr;

      tmp_arr->arr_len = 0;
      tmp_arr->arr_cap = DEFAULT_ARR_CAP;
      tmp_arr->val.j_array = new Json *[tmp_arr->arr_cap];

      parseArray(tmp_arr, json_src, i, json_size);
      delete tmp_key;
      obj->addArray(tmp_arr);
      is_key = true;
      if (json_src[*i] != ',') {
        delete cur;
        *i += 1;
        return;
      }
      *i += 1;
    } break;
    case '{': {
      *i += 1;
      Json *tmp_obj;
      tmp_obj->type = JSON_OBJECT;
      tmp_obj->key = tmp_key;
      tmp_obj->root = false;

      tmp_obj->obj_len = 0;
      tmp_obj->obj_cap = DEFAULT_OBJ_CAP;
      tmp_obj->val.j_obj = new Json *[tmp_obj->obj_cap];

      parseObj(tmp_obj, json_src, i, json_size);
      obj->addObj(tmp_obj);
      is_key = true;
      if (json_src[*i] != ',') {
        delete cur;
        *i += 1;
        return;
      }
      *i += 1;
    } break;
    default:
      *i += 1;
    }
  }
}

Json::Val::Val() {
  
}

Json::Val::~Val() {
  
}

Json::Json(JsonType type, const i8 *key, ...) {
  this->type = type;
  switch(type) {
  case JSON_NUMBER: {
    this->key = new String;
    this->key->concat(key);
    va_list li;
    va_start(li, key);
    f64 val = va_arg(li, f64);
    this->val.j_number = val;
    va_end(li);
  } break;
  case JSON_STRING: {
    this->key = new String;
    this->key->concat(key);
    va_list li;
    va_start(li, key);
    i8 *val = va_arg(li, i8 *);
    this->val.j_string = new String;
    this->val.j_string->concat(val);
    va_end(li);
  } break;
  case JSON_BOOLEAN: {
    this->key = new String;
    this->key->concat(key);
    va_list li;
    va_start(li, key);
    bool val = bool(va_arg(li, i32));
    this->val.j_boolean = val;
    va_end(li);
  } break;
  case JSON_ARRAY: {
    if (key == nullptr) {
      this->root = true;
    } else {
      this->key = new String;
      this->key->concat(key);
    }
    this->arr_len = 0;
    this->arr_cap = DEFAULT_ARR_CAP;
    this->val.j_array = new Json* [this->arr_cap];
  } break;
  case JSON_OBJECT: {
    if (key == nullptr) {
      this->root = true;
    } else {
      this->key = new String;
      this->key->concat(key);
    }
    this->obj_len = 0;
    this->obj_cap = DEFAULT_OBJ_CAP;
    this->val.j_obj = new Json* [this->obj_cap];
  } break;
  case JSON_NULL: {
    this->key = new String;
    this->key->concat(key);
    this->val.j_null = nullptr;
  } break;
  }
}

u32 symbols_len = 6;
i8 symbols[] = "\r\n\t\b\a ";


bool spaceSymbol(i8 c) {
  for(i32 i = 0; i < symbols_len; i++) {
    if (c == symbols[i]) return true;
  }
  return false;
}

i8 *trimJsonSource(const i8 *json_src, u64 file_size, u64 *new_size) {
  i8 *prep_source = new i8 [file_size];
  u32 i = 0;
  for(u64 j = 0; j < file_size; j++) {
    if (spaceSymbol(json_src[j])) continue;
    else {
      prep_source[i++] = json_src[j];
    }
  }
  *new_size = i;
  return prep_source;
}

Json::Json() {
  ZERO(this, 1); 
}

Json::~Json() {

  if (this->key != nullptr) delete this->key;
  if (this->type == JSON_STRING) {
    if (this->val.j_string != nullptr) {
      delete this->val.j_string;
    }
  }
  if (this->type == JSON_OBJECT) {
    Json **objects = this->val.j_obj;
    for(i32 i = 0; i < this->obj_len; i++) {
      delete objects[i];
    }
    delete [] objects;
  } else if (this->type == JSON_ARRAY) {
    Json **arr = this->val.j_array;
    for(i32 i = 0; i < this->arr_len; i++) {
      delete arr[i];
    }
    delete [] arr;
  }
}

Json::Json(const i8 *file_name) {
  i32 fd = open(file_name, O_RDONLY);
  if (fd <= 0) {
    fprintf(stderr, "Can't open file %s\n", file_name);
    return;
  }

  u64 file_size = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  i8 *json_src = new i8 [file_size + 1];
  ZERO(json_src, file_size + 1);
  u64 read_bytes = read(fd, json_src, file_size);
  if (read_bytes != file_size) {
    fprintf(stderr, "Read bytes %ld, not eql file_size %ld\n", read_bytes, file_size);
    return;
  }

  close(fd);

  u64 *i = new u64;
  *i = 1;
  u64 new_size = 0;

  i8 *prep_json_src = trimJsonSource(json_src, file_size, &new_size);
  delete [] json_src;

  if (prep_json_src[0] == '{') {
    this->type = JSON_OBJECT;
    this->root = true;

    this->obj_len = 0;
    this->obj_cap = DEFAULT_OBJ_CAP;
    this->val.j_obj = new Json *[this->obj_cap];


    parseObj(this, prep_json_src, i, new_size);
  } else if (prep_json_src[0] == '[') {
    this->type = JSON_ARRAY;
    this->root = true;

    this->arr_len = 0;
    this->arr_cap = DEFAULT_OBJ_CAP;
    this->val.j_array = new Json *[this->arr_cap];
    parseArray(this, prep_json_src, i, new_size);
  }
  delete i;
  delete [] prep_json_src;

}

Json::Json(const i8 *buf, u32 buf_len) {
  u64 i = 0;
  u64 new_size = 0;

  i8 *prep_json_src = trimJsonSource(buf, buf_len, &new_size);

  this->root = true;
  parseObj(this, prep_json_src, &i, new_size);
  delete [] prep_json_src;
}

void Json::checkObjValSize() {
  if (this->obj_len == this->obj_cap) {
    this->obj_cap <<= 1;
    Json **new_obj = new Json* [this->obj_cap];
    for(i32 i = 0; i < this->obj_len; i++) {
      new_obj[i] = this->val.j_obj[i];
    }
    delete [] this->val.j_obj;
    this->val.j_obj = new_obj;
  }
}
void Json::checkArrValSize() {
  if (this->arr_len == this->arr_cap) {
    this->arr_cap <<= 1;
    Json **new_arr = new Json* [this->arr_cap];
    for(i32 i = 0; i < this->arr_len; i++) {
      new_arr[i] = this->val.j_array[i];
    }
    delete [] this->val.j_array;
    this->val.j_array = new_arr;
  }  
}

void Json::addString(const i8 *key, const i8 *val) {
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = new Json{JSON_STRING, key, val};
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_STRING) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = new Json{JSON_STRING, nullptr, val};
    checkArrValSize();
  }

}

void Json::addString(Json *j_string){
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = j_string;
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_STRING) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = j_string;
    checkArrValSize();
  }
}

void Json::addNumber(const i8 *key, f64 val){
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = new Json{JSON_NUMBER, key, val};
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_NUMBER) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = new Json{JSON_NUMBER, nullptr, val};
    checkArrValSize();
  }
}

void Json::addNumber(Json *j_number){
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = j_number;
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_NUMBER) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = j_number;
    checkArrValSize();
  }
}

void Json::addBoolean(const i8 *key, bool val){
if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = new Json{JSON_BOOLEAN, key, val};
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_BOOLEAN) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = new Json{JSON_BOOLEAN, nullptr, val};
    checkArrValSize();
  }  
}

void Json::addBoolean(Json *j_boolean) {
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = j_boolean;
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_NUMBER) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = j_boolean;
    checkArrValSize();
  }  
}

void Json::addArray(Json *val) {
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = val;
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_ARRAY) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = val;
    checkArrValSize();
  }
}

void Json::addObj(Json *val) {
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = val;
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_OBJECT) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = val;
    checkArrValSize();
  }
}

void Json::addNull(const i8 *key) {
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = new Json(JSON_NULL, key);
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_NULL) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = new Json(JSON_NULL, nullptr);
    checkArrValSize();
  }
}
void Json::addNull(Json *j_null) {
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = j_null;
    checkObjValSize();
  } else {
    if (this->arr_type != JSON_NULL) {
      fprintf(stderr, "Try to add String obj to not a string array\n");
      return;
    }
    this->val.j_array[this->arr_len++] = j_null;
    checkArrValSize();
  }
}

Json *Json::getObj(const i8 *key) {
  if (this->type != JSON_OBJECT) {
    fprintf(stderr, "func getObj works only with objects");
    return nullptr;
  }

  Json **obj_cont = this->val.j_obj;
  for(i32 i = 0; i < this->obj_len; i++) {
    if (*obj_cont[i]->key == key) {
      return obj_cont[i];
    }
    if (obj_cont[i]->type == JSON_OBJECT) {
      Json *res = obj_cont[i]->getObj(key);
      if (res != nullptr) return res;
    }
  }
  
  return nullptr;
}

void Json::writeToFile(const i8 *file_name) {
  String json_content = this->toString(0);

  i32 fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 666);

  if (fd <= 0) {
    fprintf(stderr, "Can't open/create file %s for json\n", file_name);
    return;
  }

  u32 write_bytes = write(fd, json_content.data, json_content.len);
  if (write_bytes != json_content.len) {
    fprintf(stderr, "write failed, write bytes %d != json_content.len %d\n", write_bytes, json_content.len);
    close(fd);
    return;
  }
  close(fd);
}

void Json::print() {
  
}


void add_inner_tab(StrBuf &sb, i32 inner_tab) {
  for (i32 i = 0; i < inner_tab; i++) {
    sb << "  ";
  }

}


String Json::toString(i32 inner_tab) {
  StrBuf sb{};

  if (this->type == JSON_OBJECT) {
    sb << "{\n";
    inner_tab++;

    Json **obj_cont = this->val.j_obj;
    for(i32 i = 0; i < this->obj_len; i++) {
      switch(obj_cont[i]->type) {
      case JSON_NUMBER: {
          add_inner_tab(sb, inner_tab);
          sb.appendFmt("\"%s\": %lf", obj_cont[i]->key->data, obj_cont[i]->val.j_number);
      } break;
      case JSON_STRING: {
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("\"%s\": \"%s\"", obj_cont[i]->key->data, obj_cont[i]->val.j_string->data);
      } break;
      case JSON_BOOLEAN: {
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("\"%s\": %s", obj_cont[i]->key->data, obj_cont[i]->val.j_boolean == true ? "true" : "false");
      } break;
      case JSON_NULL: {
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("\"%s\": null", obj_cont[i]->key->data);
      } break;
      case JSON_OBJECT: {
        String obj = obj_cont[i]->toString(inner_tab);
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("\"%s\": %s", obj_cont[i]->key->data, obj.data);
      } break;
      case JSON_ARRAY: {
        String arr = obj_cont[i]->toString(inner_tab);
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("\"%s\": %s", obj_cont[i]->key->data, arr.data);
      } break;
      }

      if (i != this->obj_len - 1) {
        sb << ",\n";
      } else {
        sb << '\n';
      }
    }

    sb << '}';
    
  } else if (this->type == JSON_ARRAY) {

    sb << "[\n";
    inner_tab++;

    Json **arr_cont = this->val.j_array;
    for(i32 i = 0; i < this->arr_len; i++) {
      switch(this->arr_type) {
      case JSON_NUMBER: {
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("%lf", arr_cont[i]->val.j_number);
      } break;
      case JSON_STRING: {
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("\"%s\"", arr_cont[i]->val.j_string->data);
      } break;
      case JSON_BOOLEAN: {
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("%s", arr_cont[i]->val.j_boolean == true ? "true" : "false");
      } break;
      case JSON_NULL: {
        add_inner_tab(sb, inner_tab);
        sb << "null";
      } break;
      case JSON_OBJECT: {
        String obj = arr_cont[i]->toString(inner_tab);
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("%s", obj.data);
      } break;
      case JSON_ARRAY: {
        String arr = arr_cont[i]->toString();
        add_inner_tab(sb, inner_tab);
        sb.appendFmt("%s", arr.data);
      } break;
      }

      if (i != this->obj_len - 1) {
        sb << ",\n";
      } else {
        sb << '\n';
      }

      sb << ']';
    }
    }

    return sb.toString();
}
