//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_UNISTD_HXX_
#define MRR_UNISTD_HXX_

#include <unistd.h>

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ssize_t read(int fildes, void* buf, size_t nbyte)
{
  ssize_t numbytes = 0;

  // Loop while the read function is interupted.  Set errno in the loop
  // to make sure that another system call has not set it.
  do
  {
    errno = 0;
    numbytes = ::read(fildes, buf, nbyte);
  }
  while (numbytes == -1 && errno == EINTR);
  return numbytes;
}



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
ssize_t write(int fildes, void const* buf, size_t nbyte)
{
  // Loop while the write function is interupted.  Set errno in the loop
  // to make sure that another system call has not set it.
  ssize_t numbytes = 0;
  do
  {
    errno = 0;
    numbytes = ::write(fildes, buf, nbyte);
  }
  while (numbytes == -1 && errno == EINTR);
  return numbytes;
}



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int close(int fd)
{
  int ret_val;
  do
  {
    errno = 0;
    ret_val = ::close(fd);
  } while(ret_val == -1 && errno != EINTR);

  return ret_val;
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#endif // #ifndef MRR_UNISTD_HXX_
