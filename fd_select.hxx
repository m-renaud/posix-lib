//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

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
    : timeout_is_set(false)
  {
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    FD_ZERO(&exceptset);
  }

  void set_timeout(long sec, long usec)
  {
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    timeout_is_set = true;
  }

  int multiplex()
  {
    int retval;

    read_checkset = readset;
    write_checkset = writeset;
    except_checkset = exceptset;

    errno = 0;
    retval = ::select(
      FD_SETSIZE,
      &read_checkset,
      &write_checkset,
      &except_checkset,
      (timeout_is_set) ? &timeout : NULL   // Optionally set a timeout
    );

    if(retval == -1)
      return -1;

    std::size_t pos = 0;

    for(int fd : read_fds)
    {
      if(FD_ISSET(fd, &read_checkset))
	read_actions[pos](fd);
      ++pos;
    }

    pos = 0;
    for(int fd : write_fds)
    {
      if(FD_ISSET(fd, &write_checkset))
	write_actions[pos](fd);
      ++pos;
    }

    pos = 0;
    for(int fd : except_fds)
    {
      if(FD_ISSET(fd, &except_checkset))
	except_actions[pos](fd);
      ++pos;
    }

    return retval;

  } // int listen_with_action()


  void register_read_fd(int fd, function_type const& f)
  {
    read_fds.push_back(fd);
    FD_SET(fd, &readset);
    read_actions.push_back(f);
  }

  void register_write_fd(int fd, function_type const& f)
  {
    write_fds.push_back(fd);
    FD_SET(fd, &writeset);
    write_actions.push_back(f);
  }

  void register_except_fd(int fd, function_type const& f)
  {
    except_fds.push_back(fd);
    FD_SET(fd, &exceptset);
    except_actions.push_back(f);
  }

private:
  fd_set readset, read_checkset;
  fd_set writeset, write_checkset;
  fd_set exceptset, except_checkset;

  bool timeout_is_set;
  struct timeval timeout;

  std::vector<int> read_fds;
  std::vector<function_type> read_actions;

  std::vector<int> write_fds;
  std::vector<function_type> write_actions;

  std::vector<int> except_fds;
  std::vector<function_type> except_actions;

}; // struct fd_select



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_POSIX_FD_SELECT_HXX_
