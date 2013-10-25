//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
// Copyright (c) 2012 Paul Preney. All Rights Reserved.
// The code in this file is based on code written by Paul Preney.
// website: http://preney.ca
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_POSIX_CXX_SYSTEM_ERROR_HXX_
#define MRR_POSIX_CXX_SYSTEM_ERROR_HXX_

#include <cerrno>
#include <system_error>
#include <type_traits>


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

template <typename... Args>
void throw_system_error(
	typename std::remove_reference<decltype(errno)>::type const errval,
	Args&&... args)
{
	//
	// See C++11 Sect. "19.5.1.5 Error category objects
	// [syserr.errcat.objects]" clause 4 paragraph 2 and note
	// that it is implementation specific WHICH category is
	// returned (i.e., generic or system). The paragraph's text
	// however implies that the code below is sufficient to
	// use system_error with any errno value (POSIX or OS-specific).
	//
	throw
		std::system_error{
		errval,
			std::system_category().default_error_condition(errval).category(),
			std::forward<Args>(args)...
			}
	;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void output_system_error_msg()
{
	try
	{
		throw;
	}
	catch (std::system_error const& e)
	{
		auto c = e.code();
		std::cout << "errno = "     << c.value() << "..."  << std::endl
		          << "      What: " << e.what()            << std::endl
		          << "  Category: " << c.category().name() << std::endl
		          << "     Value: " << c.value()           << std::endl
		          << "   Message: " << c.message()         << std::endl
		          << std::endl
			;
	}
}

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_POSIX_CXX_SYSTEM_ERROR_HXX_
