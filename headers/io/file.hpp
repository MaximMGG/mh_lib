#ifndef MH_LIB_FILE_HPP
#define MH_LIB_FILE_HPP
#include "../core/string.hpp"
#include "../core/types.hpp"
#include <fcntl.h>
#include <unistd.h>

enum File_Seek_Pos { FILE_SET = 0, FILE_CUR = 1, FILE_END = 2 };
//
// #define O_RDONLY	     00
// #define O_WRONLY	     01
// #define O_RDWR		     02
// #ifndef O_CREAT
// # define O_CREAT	   0100	/* Not fcntl.  */
// #endif
// #ifndef O_EXCL
// # define O_EXCL		   0200	/* Not fcntl.  */
// #endif
// #ifndef O_NOCTTY
// # define O_NOCTTY	   0400	/* Not fcntl.  */
// #endif
// #ifndef O_TRUNC
// # define O_TRUNC	  01000	/* Not fcntl.  */
// #endif
// #ifndef O_APPEND
// # define O_APPEND	  02000
// #endif
// #ifndef O_NONBLOCK
// # define O_NONBLOCK	  04000
// #endif

enum File_Open_Type {
  FILE_READ,
  FILE_WRITE,
  FILE_CREATE = 100,
  FILE_TRUNC = 1000,
  FILE_APPEND = 2000,
};

enum File_Mode {
  URDWRX = 700,
  URDWR = 600,
  UREAD = 400,
  UWRITE = 200,
  UEXECUTE = 100,
  GRDWRX = 070,
  GRDWR = 060,
  GREAD = 040,
  GWRITE = 020,
  GEXECUTE = 010,
  ORDWRX = 007,
  ORDWR = 060,
  OREAD = 004,
  OWRITE = 002,
  OEXECUTE = 001,
};

#define DEF_FILE_MODE URDWR | GRDWR | ORDWR

class File {
public:
  String path;
  i32 fd = 0;
  u64 offset = 0;
  u32 file_size = 0;
  i32 mode = 0;
  i32 flag = 0;
  bool ok = true;
  File_Open_Type open_type;

  File(const i8 *path);

  void open(i32 flags);
  void trunc();

  void create(i32 flags, i32 mode);

  String readEntyreFile();

  String read(u64 size);
  void read(u64 size, i8 *buf);
  String readLine();
  i8 readChar();
  u64 seek(u64 offset, File_Seek_Pos position);
  String readUntilDelimeter(i8 del);
  u64 size();

  void write(u64 size, i8 *buf);
  void write(String s);

  void close();

  ~File();

private:
  i8 buffer[4096];
};

#endif // MH_LIB_FILE_HPP
