//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_FILE_DESCRIPTOR_HXX_
#define MRR_FILE_DESCRIPTOR_HXX_

#include "unistd.hxx"

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct pipe;

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct file_descriptor
{
  friend class pipe;

  using fd_type = int;

  file_descriptor()
    : file_descriptor(-1)
  {
  }

  file_descriptor(fd_type fd_, bool auto_close_fd_ = false)
    : fd(fd_), auto_close_fd(auto_close_fd_)
  {
  }

  void auto_close(bool auto_close_fd_)
  {
    auto_close_fd = auto_close_fd_;
  }

  bool auto_close() const
  {
    return auto_close_fd;
  }

  int close()
  {
    int ret_val;

    if(fd == -1)
      return;

    ret_val = mrr::posix::close(fd);

    if(ret_val != -1)
      fd = -1;

    return ret_val;
  }

  ~file_descriptor()
  {
    if(auto_close_fd)
    {
      int ret_val = this->close();
    }
  }

  file_descriptor& operator =(fd_type const& fd_)
  {
    fd = fd_;
    return *this;
  }

  operator fd_type const&() const
  {
    return fd;
  }

  fd_type const& get_fd() const
  {
    return fd;
  }

private:
  fd_type fd;
  bool auto_close_fd;

}; // struct file_descriptor


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_FILE_DESCRIPTOR_HXX_
