//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_SIGNAL_SET_HXX_
#define MRR_SIGNAL_SET_HXX_

#include <signal.h>

#include "cxx_system_error.hxx"
#include "utility.hxx"

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
struct signal_set
{
	signal_set()
		: good_state{true}
	{
		empty();
	}

	void empty()
	{
		if(!good_state)
			return;

		errno = 0;
		::sigemptyset(&set);
	}

	void fill()
	{
		if(!good_state)
			return;

		errno = 0;
		::sigfillset(&set);
	}

	int add(int signum)
	{
		if(!good_state)
			return -1;

		errno = 0;
		int ret_val = ::sigaddset(&set, signum);

		if(ret_val == -1)
			good_state = false;

		return ret_val;
	}

	int del(int signum)
	{
		if(!good_state)
			return -1;

		errno = 0;
		int ret_val = ::sigdelset(&set, signum);

		if(ret_val == -1)
			good_state = false;

		return ret_val;
	}

	bool contains(int signum) const
	{
		if(!good_state)
			return false;

		errno = 0;
		int ret_val = ::sigismember(&set, signum);

		if(ret_val == 1)
			return true;
		else
			return false;
	}

	explicit operator bool() const
	{
		return good_state;
	}

private:
	sigset_t set;
	bool good_state;

}; // struct signal_set



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#endif // #ifndef MRR_SIGNAL_SET_HXX_
