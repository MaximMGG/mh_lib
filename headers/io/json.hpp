#include "../core.hpp"


typedef enum {
  JSON_STRING,
  JSON_NUMBER,
  JSON_ARRAY,
  JSON_OBJECT,
  JSON_BOOLEAN,
  JSON_NULL
} JsonType;


struct Json {
  String key;
  
  union Val {
    String j_string;
    f64 j_number;
    Json **j_array;
    Json **j_obj;
    bool j_boolean;
    ptr j_null;
    Val();
    ~Val();
  } val;  
  
  
  JsonType val_type;
  JsonType arr_type;

  u32 obj_len;
  u32 obj_cap;

  u32 arr_len;
  u32 arr_cap;
  bool root = false;

  Json(JsonType type, const i8 *key, ...);
  Json(const i8 *file_name);
  Json(const i8 *buf, u32 buf_len);
  Json();
  ~Json();

  void addString(const i8 *key, const i8 *val);
  void addString(Json *j_string);
  void addNumber(const i8 *key, f64 val);
  void addNumber(Json *j_number);
  void addBoolean(const i8 *key, bool val);
  void addBoolean(Json *j_boolean);
  void addArray(const i8 *key, Json *val);
  void addArray(Json *j_array);
  void addObj(const i8 *key, Json *val);
  void addObj(Json *j_obj);
  void addNull(const i8 *key);
  void addNull(Json *j_null);
  

  Json *getObj(const i8 *key);

  void writeToFile(const i8* file_name);
  void print();
  void toString();

private:
  void checkObjValSize();
  void checkArrValSize();
};
