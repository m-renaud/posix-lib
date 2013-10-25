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

#include "cxx_system_error.hxx"
#include "utility.hxx"


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
		struct sigaction& action = sigaction_structs[signo];
		action.sa_handler = SIG_IGN;
		return sigaction(signo, &action, NULL);
	}

	int set_handle(int signo, std::function<void()> const& f, int flags = 0)
	{
		int return_value;

		struct sigaction& action = sigaction_structs[signo];
		action.sa_handler = global_signal_handling_function;
		action.sa_flags = flags;
		return_value = ::sigaction(signo, &action, NULL);

		if(return_value == -1)
			mrr::posix::throw_system_error(
				errno,
				"Failed to register handler for "
				+ mrr::posix::signal_to_string(signo)
			);

		if(return_value == 0)
			handles[signo] = f;

		return return_value;
	}

	int set_mask(int signo, signal_set const& sset)
	{
		int return_value;

		struct sigaction& action = sigaction_structs[signo];
		action.sa_mask = static_cast<sigset_t>(sset);
		return_value = ::sigaction(signo, &action, NULL);

		if(return_value == -1)
			mrr::posix::throw_system_error(
				errno,
				"Failed to set signal mask for "
				+ mrr::posix::signal_to_string(signo)
			);

		return return_value;
	}

	std::unordered_map<int,std::function<void()> > handles;
	std::unordered_map<int,struct sigaction> sigaction_structs;

}; // struct signal_handler;



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
signal_handler signals;


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void global_signal_handling_function(int signo)
{
	signals.handles[signo]();
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_SIGNAL_HANDLER_HXX_
