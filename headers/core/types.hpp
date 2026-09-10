#ifndef MH_TYPES_HPP
#define MH_TYPES_HPP
#include <string.h>

typedef char           byte;
typedef char           i8;
typedef unsigned char  u8;
typedef short          i16;
typedef unsigned short u16;
typedef int            i32;
typedef unsigned int   u32;
typedef long           i64;
typedef unsigned long  u64;
typedef float          f32;
typedef double         f64;
typedef void *         ptr;

#define null NULL;

#define I8(val) static_cast<i8>(val)
#define U8(val) static_cast<u8>(val)
#define I16(val) static_cast<i16>(val)
#define U16(val) static_cast<u16>(val)
#define I32(val) static_cast<i32>(val)
#define U32(val) static_cast<u32>(val)
#define I64(val) static_cast<i64>(val)
#define U64(val) static_cast<u64>(val)
#define F32(val) static_cast<f32>(val)
#define F64(val) static_cast<f64>(val)

#define cast(type, val) ((type)val)


template <typename T>
void ZERO(T *t, u32 size) {
  memset(t, 0, sizeof(T) * size);
}

template <typename T>
class Optional {
public:
  bool ok;
  Optional(T *element) {
    this->ok = true;
    this->element = element;
  }
  Optional() : ok(false){
    this->element = NULL;
  }

  Optional(Optional<T> &o) {
    this->element = o.element;
    this->ok = o.ok;
  }

  T& operator&() {
    return *this->element;
  }

  void operator=(Optional<T> other) {
    this->element = other.element;
    this->ok = other.ok;
  }

private:
  T *element;
};



#endif //MH_TYPES_HPP
