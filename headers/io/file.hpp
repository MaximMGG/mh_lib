#ifndef MH_LIB_FILE_HPP
#define MH_LIB_FILE_HPP
#include "../core/types.hpp"
#include "../core/string.hpp"
#include <fcntl.h>
#include <unistd.h>


enum File_Seek_Pos {
  FILE_CUR, FILE_SET, FILE_END
};

enum File_Open_Type {

};

#define FMODE_USER_READ       b100000000
#define FMODE_USER_WRITE      b010000000
#define FMODE_USER_EXECUTE    b001000000
#define FMODE_GROUP_READ      b000100000
#define FMODE_GROUP_WRITE     b000010000
#define FMODE_GROUP_EXECUTE   b000001000
#define FMODE_OTHER_READ      b000000100
#define FMODE_OTHER_WRITE     b000000010
#define FMODE_OTHER_EXECUTE   b000000001

#define FMODE_USER_RDWR       FMODE_USER_READ | FMODE_USER_WRITE

#define FMODE_DEFAULT         b110110110

class File {
public:
  String path;
  i32 fd = 0;
  u64 offset = 0;
  u32 size = 0;
  u64 file_mode = 0;
  File_Open_Type open_type;

  File() = default;
  File(const i8 *path) : path(path){

  }

  String readEntyreFile();

  String read(u64 size);
  void   read(u64 size, i8 *buf);
  String readLine();
  i8     readChar();
  u64    seek(u64 offset, File_Seek_Pos);
  String readUntilDelimeter(i8 del);


  void write(u64 size, i8 *buf);
  void write(String s);

  void close();

  ~File();
  
private:
  i8 buffer[4096];
};

#endif //MH_LIB_FILE_HPP
