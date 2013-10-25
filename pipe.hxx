//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_PIPE_HXX_
#define MRR_PIPE_HXX_

#include <array>
#include <unistd.h>

#include "file_descriptor.hxx"
#include "cxx_system_error.hxx"

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct pipe
{
	using fd_type = file_descriptor::fd_type;

private:
	void init()
	{
		int fd_[2];
		int ret_val = ::pipe(fd_);

		if(ret_val == -1)
			mrr::posix::throw_system_error(
				errno,
				"Failed to create pipe"
			);

		read = fd_[0];
		write = fd_[1];
	}

public:
	pipe()
	{
		init();
	}

	pipe(pipe&& p)
		: read(p.read_end()), write(p.write_end())
	{
		p.read = -1;
		p.write = -1;
	}

	pipe& operator =(pipe&& p)
	{
		read = p.read_end();
		write = p.write_end();

		p.read = -1;
		p.write = -1;

		return *this;
	}

	pipe(pipe const&) = delete;
	pipe& operator =(pipe const&) = delete;

	void close()
	{
		read.close();
		write.close();
	}

	void close_read_end()
	{
		read.close();
	}

	void close_write_end()
	{
		write.close();
	}

	void create()
	{
		this->close();
		init();
	}

	file_descriptor& read_end()
	{
		return read;
	}

	file_descriptor& write_end()
	{
		return write;
	}

private:
	file_descriptor read;
	file_descriptor write;
};


pipe make_pipe()
{
	return pipe();
}

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif // #ifndef MRR_PIPE_HXX_
