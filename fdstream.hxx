#ifndef MRR_FDSTREAM_HXX_
#define MRR_FDSTREAM_HXX_

#include <iostream>
#include <streambuf>
#include <cstdio>
#include <unistd.h>
#include <cstring>

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace detail {

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Wrapper function for the read system call to handle EINTR.
// The read function is reentrant so this must be taken care of...
ssize_t myread(int fildes, void *buf, size_t nbyte)
{
  ssize_t numbytes = 0;

  // Loop while the read function is interupted.  Set errno in the loop
  // to make sure that another system call has not set it.
  do
  {
    errno = 0;
    numbytes = read(fildes, buf, nbyte);
  }
  while (numbytes == -1 && errno == EINTR);
  return numbytes;
}

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace detail

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class fd_out_buf : public std::streambuf
{
protected:
  int fd;

public:
  fd_out_buf(int fd_)
    : fd(fd_)
  {
  }

protected:
  virtual int_type overflow(int_type c)
  {
    if(c != EOF)
    {
      char z = c;
      if(write (fd, &z, 1) != 1)
      {
	return EOF;
      }
    }
    return c;
  }

  virtual std::streamsize xsputn(const char* str, std::streamsize n)
  {
    return write(fd,str,n);
  }
  
}; // class fd_out_buf


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class fd_ostream : public std::ostream
{
protected:
  fd_out_buf buf;
public:
  fd_ostream (int fd)
    : std::ostream(0), buf(fd)
  {
    rdbuf(&buf);
  }

}; // class fd_ostream



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class fd_in_buf : public std::streambuf
{

protected:
  static constexpr int buffer_size = 100;
  static constexpr int putback_size = 10;
  char buffer[buffer_size];
  int fd;


public:
  fd_in_buf(int fd_)
    : fd(fd_)
  {
    setg(buffer+putback_size,
	 buffer+putback_size,
	 buffer+putback_size);
  }
  
protected:
  virtual std::streamsize showmanyc()
  {
    return this->egptr() - this->gptr();
  }

  //m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
  virtual int_type underflow()
  {
    if(gptr() < egptr())
    {
      return traits_type::to_int_type(*gptr());
    }

    int num_putback;
    num_putback = gptr() - eback();
    if(num_putback > putback_size)
      num_putback = putback_size;

    std::memmove(buffer+(putback_size - num_putback),
		 gptr()-num_putback,
		 num_putback);


    // Read new characters...
    auto num = detail::myread(fd, buffer+putback_size, buffer_size-putback_size);
    // auto num = ::read(fd, buffer+putback_size, buffer_size-putback_size);

    if(num <= 0)
    {
      return traits_type::eof();
    }

    setg(buffer+(putback_size - num_putback),
	 buffer+putback_size,
	 buffer+putback_size+num);

    return traits_type::to_int_type(*gptr());

  } // virtual int_type underflow()

}; // class fd_in_buf



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
class fd_istream : public std::istream
{
protected:
  fd_in_buf buf;

public:
  fd_istream(int fd)
    : std::istream(0), buf(fd)
  {
    rdbuf(&buf);
  }
};


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#endif // #ifndef MRR_FDSTREAM_HXX_
