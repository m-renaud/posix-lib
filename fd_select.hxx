#ifndef MRR_POSIX_FD_SELECT_HXX_
#define MRR_POSIX_FD_SELECT_HXX_

#include <sys/select.h>
#include <functional>
#include <vector>

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct fd_select
{
  using function_type = std::function<void(int)>;

  fd_select()
  {
    FD_ZERO(&readset);
  }

  int listen()
  {
    int retval;

    checkset = readset;
    do
    {
      errno = 0;
      retval = ::select(FD_SETSIZE, &checkset, NULL, NULL, NULL);
    } while(retval == -1 && errno == EINTR);

    if(retval == -1)
      return -1;

    for(int fd : fds)
      if(FD_ISSET(fd, &checkset))
	return fd;
  }

  int listen_with_action()
  {
    int retval;

    checkset = readset;
    do
    {
      errno = 0;
      retval = ::select(FD_SETSIZE, &checkset, NULL, NULL, NULL);
    } while(retval == -1 && errno == EINTR);

    if(retval == -1)
      return -1;

    std::size_t pos = 0;

    for(int fd : fds)
    {
      if(FD_ISSET(fd, &checkset))
	actions[pos](fd);
      ++pos;
    }

    return retval;

  } // int listen_with_action()


  void watch(int fd)
  {
    fds.push_back(fd);
    FD_SET(fd, &readset);
  }

  void watch(int fd, function_type&& f)
  {
    watch(fd);
    actions.push_back(f);
  }

private:

  fd_set readset, checkset;
  std::vector<int> fds;
  std::vector<function_type> actions;

}; // struct fd_select



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_POSIX_FD_SELECT_HXX_
