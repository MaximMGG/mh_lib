#include "../../headers/io/json.hpp"

#define DEFAULT_OBJ_CAP 8
#define DEFAULT_ARR_CAP 8



Json::Json(JsonType type, const i8 *key, ...) {
  switch(type) {
  case JSON_NUMBER: {
    
  } break;
  case JSON_STRING: {
    
  } break;
  case JSON_BOOLEAN: {
    
  } break;
  case JSON_ARRAY: {
    this->arr_len = 0;
    this->arr_cap = DEFAULT_ARR_CAP;
    this->val.j_array = new Json* [this->arr_cap];
  } break;
  case JSON_OBJECT: {
    if (key == nullptr) {
      this->root = true;
    } else {
      this->key = key;
    }
    this->obj_len = 0;
    this->obj_cap = DEFAULT_OBJ_CAP;
    this->val.j_obj = new Json* [this->obj_cap];
  } break;
  case JSON_NULL: {
              
  } break;
  }
}
Json::Json(const i8 *file_name) {}
Json::Json(const i8 *buf, u32 buf_len) {}
Json::~Json() {}

void Json::addString(const i8 *key, const i8 *val) {}
void Json::addNumber(const i8 *key, f64 val) {}
void Json::addBoolean(const i8 *key, bool val) {}
void Json::addArray(const i8 *key, Json *val) {}
void Json::addObj(const i8 *key, Json *val) {}
void Json::addNull(const i8 *key) {}
Json *Json::getObj(const i8 *key) {
  return nullptr;
}
void Json::writeToFile(const i8 *file_name) {}
void Json::print() {}
void Json::toString() {}
