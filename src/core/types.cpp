#include "../../headers/core/types.hpp"

template <typename T>
void ZERO(T *t, u32 size) {
  if (((sizeof(*t) * size) % 8) == 0) {
    u64 *buf = cast(u64 *, t);
    u32 len = (sizeof(*t) * size) / 8;
    while((len--) > 0) *(buf++) = 0;
  } else if (((sizeof(*t) * size) % 4) == 0) {
    u32 *buf = cast(u32 *, t);
    u32 len = (sizeof(*t) * size) / 8;
    while((len--) > 0) *(buf++) = 0;
  } else {
    u32 len = sizeof(t) * size;
    i8 *buf = cast(i8 *, t);
    while ((len--) != 0) *(t++) = 0;
  }
}
