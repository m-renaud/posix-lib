//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_SIGNAL_HANDLER_HXX_
#define MRR_SIGNAL_HONDLER_HXX_


#include <functional>
#include <unordered_map>

#include <signal.h>


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void global_signal_handling_function(int signo);


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct signal_handler
{
  int ignore(int signo)
  {
    struct sigaction act;
    act.sa_handler = SIG_IGN;
    return sigaction(signo, &act, NULL);
  }

  int set_handle(int signo, std::function<void()> const& f, int flags = 0)
  {
    int return_value;

    struct sigaction act;
    act.sa_handler = global_signal_handling_function;
    act.sa_flags = flags;
    return_value = sigaction(signo, &act, NULL);

    if(return_value == 0)
      handles[signo] = f;

    return return_value;
  }

  std::unordered_map<int,std::function<void()> > handles;

}; // struct signal_handler;



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
signal_handler signals;


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void global_signal_handling_function(int signo)
{
  signals.handles[signo]();
  struct sigaction act;
  act.sa_handler = global_signal_handling_function;
  sigaction(signo, &act, NULL);
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_SIGNAL_HANDLER_HXX_
