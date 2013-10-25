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

#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include "cxx_system_error.hxx"

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
int open(char const* pathname, int flags, mode_t mode = S_IRUSR | S_IWUSR)
{
	int ret_val;
	do
	{
		errno = 0;
		ret_val = ::open(pathname, flags, mode);
	} while(ret_val == -1 && errno == EINTR);

	if(ret_val == -1)
		mrr::posix::throw_system_error(
			errno,
			"Open failed for file " + std::string(pathname)
		);

	return ret_val;
}



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

	if(numbytes == -1)
		mrr::posix::throw_system_error(
			errno,
			"Read failed on file descriptor " + std::to_string(fildes)
		);

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

	if(numbytes == -1)
		mrr::posix::throw_system_error(
			errno,
			"Write failed on file descriptor " + fildes
		);

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
	} while(ret_val == -1 && errno == EINTR);

	if(ret_val == -1)
		mrr::posix::throw_system_error(
			errno,
			"Failed to close file descriptor " + std::to_string(fd)
		);

	return ret_val;
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#endif // #ifndef MRR_UNISTD_HXX_
