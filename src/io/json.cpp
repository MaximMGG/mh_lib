#include "../../headers/io/json.hpp"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_OBJ_CAP 8
#define DEFAULT_ARR_CAP 8
#define MAX_STRING_LENGTH 10240


class Parser {
public:
  i8 *buf;
  u32 buf_size;
  u32 i;
  bool valid;
  bool end_of_buf;
  bool inside_string;
  
  Parser(i8 *buf, u32 buf_size) : buf(buf), buf_size(buf_size){
    i = 0;
    valid = true;
    end_of_buf = false;
    inside_string = false;
  }
  ~Parser() {
    
  }
  
  void skipTo(i8 c) {
    while(this->buf[this->i] != c) {
      this->i++;
    }
  }

  i8 currentToken() {
    return this->buf[this->i];
  }

  i8 nextToken() {
    if (this->i == this->buf_size) {
      end_of_buf = true;
      return 0;
    }

    while(true) {
      this->i++;
      if (this->i == this->buf_size) {
        end_of_buf = true;
        return 0;
      }
      switch(this->buf[this->i]) {
      case '\r':
      case '\t':
      case '\n':
      case '\a': 
      case ' ': {
        if (this->inside_string) {
          return ' ';
        } else {
          continue;
        }
      }
      default: {
        return this->buf[this->i];
      }
      }
    }
    return 0;
  }
  
  i8 *parseString() {
    this->inside_string = true;
    i8 *str = new i8 [MAX_STRING_LENGTH];
    u32 j = 0;
    ZERO(str, MAX_STRING_LENGTH);
    if (this->buf[this->i] != '"') {
      fprintf(stderr, "Wrong string token, doesn't find '\"'\n");
      delete [] str;
      this->inside_string = false;
      return nullptr;
    }
    while(!this->end_of_buf) {
      if (j == MAX_STRING_LENGTH) {
        this->valid = false;
        this->inside_string = false;
        return nullptr;
      }
      i8 t = nextToken();
      if (t == '\"') {
        if (j > 0) {
          if (str[j - 1] == '\\') {
            str[j++] = '\"';
            continue;
          }
        }

        this->inside_string = false;
        return str;
      }
      str[j++] = t;
    }
    this->inside_string = false;
    return nullptr;
  }
  
  f64 parseNumber(bool *_is_float) {
    bool is_float = false;
    i8 buf[64] = {0};
    u32 buf_i = 0;
    i8 t = this->buf[this->i];
    while(!this->end_of_buf && this->valid) {
      if ((t >= '0' && t <= '9') || (t == '.' && buf_i > 0) || (t == '0' && buf_i == 0)) {
        if (t == '.') {
          is_float = true;
        }
        buf[buf_i++] = t;
      } else {
        this->i--;
        if (is_float) {
          *_is_float = true;
          return (f64)atof(buf);
        } else {
          *_is_float = false;
          return (f64)atol(buf);
        }
      }
      t = nextToken();
    }
    return 0;
  }
  
  bool parseBool(bool cond) {
    if (cond) {
      if (strncmp(&this->buf[this->i], "true", 4) == 0) {
        this->i += 3;
        return true;
      } else {
        this->valid = false;
      }
    } else {
      if (strncmp(&this->buf[this->i], "false", 5) == 0) {
        this->i += 4;
        return false;
      } else {
        this->valid = false;
      }
    }
    return false;
  }
  
  void parseNull() {
    if (strncmp(&this->buf[this->i], "null", 4) == 0) {
      this->i += 3;
      return;
    } else {
      this->valid = false;
    }
  }
  
  Json *parseArray(Json *arr) {
    bool defined_arr_type = false;
    i8 t;

    if (this->buf[this->i] != '[') {
      fprintf(stderr, "Do not find sqare brackets on the beggining of array\n");
      delete arr;
      return null;
    }
    
    while (!this->end_of_buf && this->valid) {
      t = nextToken();
      switch (t) {
      case '"': {
        if (!defined_arr_type) {
          defined_arr_type = true;
          arr->arr_type = JSON_STRING;
        } else {
          if (arr->arr_type != JSON_STRING) {
            fprintf(stderr, "Array type is String, finc not string\n");
            skipTo(']');
            nextToken();
            return arr;
          }
        }
        i8 *val = parseString();
        arr->addString(new Json(JSON_STRING, nullptr, val));
        delete[] val;
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
        if (!defined_arr_type) {
          defined_arr_type = true;
          arr->arr_type = JSON_NUMBER;
        } else {
          if (arr->arr_type != JSON_NUMBER) {
            fprintf(stderr, "Array type is Nmber, find not number\n");
            skipTo(']');
            nextToken();
            return arr;
          }
        }
        bool is_float;
        f64 val = parseNumber(&is_float);
        arr->addNumber(new Json{JSON_NUMBER, nullptr, val});
      } break;
      case 't':
      case 'f': {
        if (!defined_arr_type) {
          defined_arr_type = true;
          arr->arr_type = JSON_BOOLEAN;
        } else {
          if (arr->arr_type != JSON_BOOLEAN) {
            fprintf(stderr, "Array type is Boolean, find not Boolean\n");
            skipTo(']');
            nextToken();
            return arr;
          }
        }
        bool val = parseBool(t == 'f' ? false : true);
        arr->addBoolean(new Json{JSON_BOOLEAN, nullptr, val});
      } break;
      case 'n': {
        if (!defined_arr_type) {
          defined_arr_type = true;
          arr->arr_type = JSON_NULL;
        } else {
          if (arr->arr_type != JSON_NULL) {
            fprintf(stderr, "Array type is Null, find not Null\n");
            skipTo(']');
            nextToken();
            return arr;
          }
        }
        parseNull();
        arr->addNull(new Json{JSON_NULL, nullptr});
      } break;
      case '[': {
        if (!defined_arr_type) {
          defined_arr_type = true;
          arr->arr_type = JSON_ARRAY;
        } else {
          if (arr->arr_type != JSON_ARRAY) {
            fprintf(stderr, "Array type is Array, find not Array\n");
            skipTo(']');
            nextToken();
            return arr;
          }
        }
        Json *val = new Json{JSON_ARRAY, nullptr};
        val->root = false;
        parseArray(val);
        arr->addArray(val);
      } break;
      case '{': {
        if (!defined_arr_type) {
          defined_arr_type = true;
          arr->arr_type = JSON_OBJECT;
        } else {
          if (arr->arr_type != JSON_OBJECT) {
            fprintf(stderr, "Array type is Object, find not Object\n");
            skipTo(']');
            nextToken();
            return arr;
          }
        }

        Json *val = new Json{JSON_OBJECT, nullptr};
        val->root = false;
        parseObj(val);
        arr->addObj(val);
      } break;
      case ']': {
        return arr;
      } break;
      }
      t = nextToken();
      if (t != ',') {
        if (t == ']') {
          return arr;
        } else {
          this->valid = false;
          delete arr;
          return nullptr;
        }
      }
    }

    return arr;
  }
  Json *parseObj(Json *obj) {
    bool find_key = false;
    i8 *key;
    i8 t;

    while (!this->end_of_buf && this->valid) {
        t = nextToken();
        switch (t) {
        case '"': {
          if (!find_key) {
            key = parseString();
            find_key = true;
            t = nextToken();
            if (t != ':') {
              this->valid = false;
              fprintf(stderr, "Json invalid, after key \"%s\", didn't find ':'\n", key);
              delete[] key;
              delete obj;
              return nullptr;
            }
            continue;
          } else {
            i8 *val = parseString();
            obj->addString(new Json{JSON_STRING, key, val});
            delete[] key;
            delete[] val;
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
          bool is_float;
          f64 val = parseNumber(&is_float);
          Json *new_num = new Json{JSON_NUMBER, key, val};
          new_num->is_float_number = is_float;
          obj->addNumber(new_num);
          delete [] key;
        } break;
        case 't':
        case 'f': {
          bool val = parseBool(t == 't' ? true : false);
          obj->addBoolean(new Json{JSON_BOOLEAN, key, val});
          delete [] key;
        } break;
        case 'n': {
          parseNull();
          obj->addNull(new Json{JSON_NULL, key});
          delete [] key;
        } break;
        case '[': {
          Json *arr = new Json{JSON_ARRAY, key};
          arr->root = false;
          parseArray(arr);
          obj->addArray(arr);
          delete [] key;
        } break;
        case '{': {
          Json *new_obj = new Json{JSON_OBJECT, key};
          new_obj->root = false;
          parseObj(new_obj);
          obj->addObj(new_obj);
          delete [] key;
        } break;
        case '}': {
          if (find_key) {
            delete key;            
          }
          return obj;
        } break;
        }
        t = nextToken();
        find_key = false;
        if (t != ',') {
          if (t == '}') {
            return obj;
          } else {
            delete obj;
            this->valid = false;
            return nullptr;
          }
        }
    }
    return obj;
  }
};


Json::Val::Val() {
  
}

Json::Val::~Val() {
  
}

Json::Json(JsonType type, const i8 *key, ...) {
  if (key != nullptr) {
    this->key = new String;
    this->key->concat(key);
  } else {
    this->key = nullptr;
  }
  this->type = type;
  switch(type) {
  case JSON_NUMBER: {
    va_list li;
    va_start(li, key);
    f64 val = va_arg(li, f64);
    this->val.j_number = val;
    bool is_float = va_arg(li, i32);
    this->is_float_number = is_float;
    va_end(li);
  } break;
  case JSON_STRING: {
    va_list li;
    va_start(li, key);
    i8 *val = va_arg(li, i8 *);
    this->val.j_string = new String;
    this->val.j_string->concat(val);
    va_end(li);
  } break;
  case JSON_BOOLEAN: {
    va_list li;
    va_start(li, key);
    bool val = bool(va_arg(li, i32));
    this->val.j_boolean = val;
    va_end(li);
  } break;
  case JSON_ARRAY: {
    this->arr_len = 0;
    this->arr_cap = DEFAULT_ARR_CAP;
    this->val.j_array = new Json* [this->arr_cap];
  } break;
  case JSON_OBJECT: {
    this->obj_len = 0;
    this->obj_cap = DEFAULT_OBJ_CAP;
    this->val.j_obj = new Json* [this->obj_cap];
  } break;
  case JSON_NULL: {
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

  Parser p{json_src, cast(u32, file_size)};


  if (p.currentToken() == '{') {
    this->type = JSON_OBJECT;
    this->root = true;

    this->obj_len = 0;
    this->obj_cap = DEFAULT_OBJ_CAP;
    this->val.j_obj = new Json *[this->obj_cap];

    p.parseObj(this);

  } else if (p.currentToken() == '[') {
    this->type = JSON_ARRAY;
    this->root = true;

    this->arr_len = 0;
    this->arr_cap = DEFAULT_OBJ_CAP;
    this->val.j_array = new Json *[this->arr_cap];
    p.parseArray(this);
  }
  delete i;
  delete [] json_src;
}

Json::Json(const i8 *buf, u32 buf_len) {
  u64 new_size = 0;

  Parser p{(i8 *)buf, buf_len};
  this->root = true;
  if (p.currentToken() == '{') {
    this->type = JSON_OBJECT;
    this->root = true;
    this->key = nullptr;
    this->obj_cap = DEFAULT_OBJ_CAP;
    this->obj_len = 0;
    this->val.j_obj = new Json*[this->obj_cap];
    p.parseObj(this);    
  } else if (p.currentToken() == '[') {
    this->type = JSON_ARRAY;
    this->arr_cap = DEFAULT_ARR_CAP;
    this->arr_len = 0;
    this->val.j_array = new Json *[this->arr_cap];
    this->root = true;
    this->key = nullptr;
    p.parseArray(this);
  } else {
    fprintf(stderr, "Incorrect Json, first token is not '{' or '['\n");
    return;
  }

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

void Json::addNumber(const i8 *key, f64 val, bool is_float){
  if (this->type == JSON_OBJECT) {
    this->val.j_obj[this->obj_len++] = new Json{JSON_NUMBER, key, val, is_float};
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

    if (this->obj_len == 0) {
      sb << "{}";
      return sb.toString();
    }
    
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

    add_inner_tab(sb, inner_tab - 1);
    sb << '}';
    
  } else if (this->type == JSON_ARRAY) {
    if (this->arr_len == 0) {
      sb << "[]";
      return sb.toString();
    }

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

    }
    
    add_inner_tab(sb, inner_tab - 1);      
    sb << ']';
    }

    return sb.toString();
}
