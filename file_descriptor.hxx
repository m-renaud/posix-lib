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

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct file_descriptor
{
  using fd_type = int;

  file_descriptor()
    : file_descriptor(-1)
  {
  }

  file_descriptor(fd_type fd_)
    : fd(fd_)
  {
  }

  int close()
  {
    if(fd == -1)
      return 0;

    int ret_val = mrr::posix::close(fd);
    fd = -1;

    return ret_val;
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

}; // struct file_descriptor


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_FILE_DESCRIPTOR_HXX_
