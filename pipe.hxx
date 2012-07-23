//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_PIPE_HXX_
#define MRR_PIPE_HXX_

#include <array>
#include <unistd.h>

#include "file_descriptor.hxx"

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct pipe
{
  using fd_type = file_descriptor::fd_type;

private:
  void init()
  {
    int fd_[2];
    ::pipe(fd_);
    read = fd_[0];
    write = fd_[1];
  }

public:
  pipe()
  {
    init();
  }

  pipe(pipe&& p)
    : read(p.read.fd), write(p.write.fd)
  {
    p.read.fd = -1;
    p.write.fd = -1;
  }

  pipe& operator =(pipe&& p)
  {
    read = p.read.fd;
    write = p.read.fd;
    p.read.fd = -1;
    p.write.fd = -1;
    return *this;
  }

  pipe(pipe const&) = delete;
  pipe& operator =(pipe const&) = delete;

  ~pipe()
  {
    this->close();
  }

  void close()
  {
    read.close();
    write.close();
  }

  void close_read_end()
  {
    read.close();
  }

  void close_write_end()
  {
    write.close();
  }

  void create()
  {
    this->close();
    init();
  }

  file_descriptor& read_end()
  {
    return read;
  }

  file_descriptor& write_end()
  {
    return write;
  }

private:
  file_descriptor read;
  file_descriptor write;
};


pipe make_pipe()
{
  return pipe();
}

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_PIPE_HXX_
