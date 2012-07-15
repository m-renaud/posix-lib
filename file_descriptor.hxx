#ifndef MRR_FILE_DESCRIPTOR_HXX_
#define MRR_FILE_DESCRIPTOR_HXX_

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
    : fd(-1)
  {
  }
  
  file_descriptor(fd_type fd_, bool close_fd_ = true)
    : fd(fd_)
  {
  }

  file_descriptor(file_descriptor&& f)
    : fd(f.fd)
  {
    f.fd = -1;
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
    this->close();
  }

  fd_type operator =(fd_type const& fd_)
  {
    this->close();
    fd = fd_;
  }

  operator fd_type&()
  {
    return fd;
  }

  fd_type& get_fd()
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
