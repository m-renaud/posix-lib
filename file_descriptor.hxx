#ifndef MRR_FILE_DESCRIPTOR_HXX_
#define MRR_FILE_DESCRIPTOR_HXX_

#include <errno.h>
#include <unistd.h>

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

  file_descriptor(file_descriptor&& f)
    : file_descriptor(f.fd)
  {
    f.fd = -1;
  }

  void auto_close(bool auto_close_fd_)
  {
    auto_close_fd = auto_close_fd_;
  }

  bool auto_close() const
  {
    return auto_close_fd;
  }

  void close()
  {
    int ret_val;
    
    if(fd == -1)
      return;
    do
    {
      errno = 0;
      ret_val = ::close(fd);
    } while(ret_val == -1 && errno != EINTR);

    fd = -1;
  }

  ~file_descriptor()
  {
    if(auto_close_fd)
      this->close();
  }

  file_descriptor& operator =(fd_type const& fd_)
  {
    this->close();
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
