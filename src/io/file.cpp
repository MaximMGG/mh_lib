#include "../../headers/io/file.hpp"
#include <sys/stat.h>

File::File(const i8 *file_name) : path(file_name){}
File::~File() {
  if (this->fd != -1) {
    ::close(this->fd);
  }
}

void File::open(i32 flags) {
  this->fd = ::open(this->path.data, flags);
  O_RDONLY;
  if (this->fd == -1) {
    this->ok = false;
    return;
  }
}

void File::trunc() {
  if (this->fd < 1) {
    ::close(this->fd);
  }
  if (this->flag == 0) {
    this->fd = ::open(this->path.data, FILE_CREATE | FILE_TRUNC, DEFFILEMODE);

  }
}

void File::create(i32 flags, i32 mode) {
  this->fd = ::open(this->path.data, O_CREAT | flags, mode);
  if (this->fd == -1) {
    this->ok = false;
    return;
  }
}

String File::readEntyreFile() {
  struct stat s;
  fstat(this->fd, &s);
  i8 *file_buf = new i8 [s.st_size + 1];
  u32 read_bytes = ::read(this->fd, file_buf, s.st_size);
  if (read_bytes != s.st_size) {
    this->ok = false;
    delete [] file_buf;
    return String();
  }
  this->file_size = s.st_size;
  this->offset += read_bytes;
  file_buf[read_bytes] = '\0';
  String res(file_buf, read_bytes);
  delete [] file_buf;
  return res;
}

String File::read(u64 size) {
  i8 *buf = new i8 [size + 1];
  u64 read_bytes = ::read(this->fd, buf, size);
  if (read_bytes != size) {
    this->ok = false;
    delete [] buf;
    return String();
  }
  buf[read_bytes] = '\0';
  this->offset += read_bytes;
  String res(buf, read_bytes);
  delete [] buf;
  return res;
}

void File::read(u64 size, i8 *buf) {
  u64 read_bytes = ::read(this->fd, buf, size);
  if (read_bytes != size) {
    this->ok = false;
  }
  this->offset += read_bytes;
}

String File::readLine() {
  return readUntilDelimeter('\n');
}

i8 File::readChar() {
  i8 c;
  u32 read_bytes = ::read(this->fd, &c, 1);
  if (read_bytes <= 0) {
    this->ok = false;
    return 0;
  }
  this->offset++;
  return c;
}

u64 File::seek(u64 offset, File_Seek_Pos position) {
  this->offset = lseek(this->fd, offset, position);
  return this->offset;
}

String File::readUntilDelimeter(i8 del) {
  struct stat s;
  fstat(this->fd, &s);
  i8 *file_buf = new i8 [s.st_size + 1];
  u64 f_index = 0;
  while(true) {
    i8 c;
    ::read(this->fd, &c, 1);
    if (c == del) {
      file_buf[f_index] = '\0';
      break;
    }
    file_buf[f_index++] = c;
  }
  this->offset = f_index + 1;
  String res(file_buf, f_index);
  delete [] file_buf;
  return res;
}

u64 File::size() {
  if (this->file_size == 0) {
    struct stat s;
    fstat(this->fd, &s);
    this->file_size = s.st_size;
    return s.st_size;
  } else {
    return this->file_size;
  }
}

void File::write(u64 size, i8 *buf) {
  u64 write_bytes = ::write(this->fd, buf, size);
  if (write_bytes != size) {
    this->ok = false;
    return;
  }
  this->offset += write_bytes;
}

void File::write(String s) {
  u64 write_bytes = ::write(this->fd, s.data, s.len);
  if (write_bytes != s.len) {
    this->ok = false;
    return;
  }
  this->offset += write_bytes;
}

void File::close() {
  ::close(this->fd);
}

