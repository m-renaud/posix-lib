//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Copyright (c) 2012, Matthew Renaud
// All rights reserved.
//
// Licensed under the FreeBSD license. See LICENSE file for details.
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef MRR_FDSTREAM_HXX_
#define MRR_FDSTREAM_HXX_

#include <iostream>
#include <streambuf>

#include "unistd.hxx"

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename CharT, class ChTr>
class fd_buf : public std::basic_streambuf<CharT,ChTr>
{

	using int_type = typename std::basic_streambuf<CharT,ChTr>::int_type;
	using char_type = typename std::basic_streambuf<CharT,ChTr>::char_type;
	using traits_type = typename std::basic_streambuf<CharT,ChTr>::traits_type;

private:
	int fd;
	char char_buf;
	bool take_from_buf;

public:
	fd_buf(int fd_)
		: fd(fd_), take_from_buf(false)
	{
	}

	fd_buf(fd_buf const&) = delete;
	fd_buf& operator =(fd_buf const&) = delete;

protected:
	int_type overflow(int_type c = traits_type::eof())
	{
		if(!traits_type::eq_int_type(c, traits_type::eof()))
		{
			errno = 0;
			char_type c2 = traits_type::to_char_type(c);
			if(mrr::posix::write(fd, &c2, 1) < 0 && errno != 0)
				return traits_type::eof();
			else
				return c;
		}

		return traits_type::not_eof(c);

	} // int_type overflow(int_type)



	int_type uflow()
	{
		if(take_from_buf)
		{
			take_from_buf = false;
			return traits_type::to_int_type(char_buf);
		}
		else
		{
			char_type c;
			errno = 0;

			if(mrr::posix::read(fd, &c, 1) < 0 && errno != 0)
			{
				return traits_type::eof();
			}
			else
			{
				char_buf = c;
				return traits_type::to_int_type(c);
			}

		}

	}  // int_type uflow()



	int_type underflow()
	{
		if(take_from_buf)
		{
			return traits_type::to_int_type(char_buf);
		}
		else
		{
			char_type c;
			errno = 0;

			if(mrr::posix::read(fd, &c, 1) < 0 && errno != 0)
			{
				return traits_type::eof();
			}
			else
			{
				take_from_buf = true;
				char_buf = c;
				return traits_type::to_int_type(c);
			}
		}

	} // int_type underflow()



	int_type pbackfail(int_type c)
	{
		if(!take_from_buf)
		{
			if(!traits_type::eq_int_type(c, traits_type::eof()))
				char_buf = traits_type::to_char_type(c);

			take_from_buf = false;
			return traits_type::to_int_type(char_buf);
		}
		else
		{
			return traits_type::eof();
		}

	} // int_type pbackfail(int_type)

}; // class fd_buf



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename CharT, typename ChTr = std::char_traits<CharT> >
class basic_fd_istream : public std::basic_istream<CharT,ChTr>
{
	using base_type = std::basic_istream<CharT,ChTr>;

protected:
	fd_buf<CharT,ChTr> buf;

public:
	basic_fd_istream(int fd)
		: std::istream(0), buf(fd)
	{
		base_type::rdbuf(&buf);
	}
};



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename CharT, typename ChTr = std::char_traits<CharT> >
class basic_fd_ostream : public std::basic_ostream<CharT,ChTr>
{
	using base_type = std::basic_ostream<CharT,ChTr>;

protected:
	fd_buf<CharT,ChTr> buf;
public:
	basic_fd_ostream (int fd)
		: std::ostream(0), buf(fd)
	{
		base_type::rdbuf(&buf);
	}

}; // class fd_ostream



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
using fd_istream = basic_fd_istream<char>;
using fd_ostream = basic_fd_ostream<char>;

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#endif // #ifndef MRR_FDSTREAM_HXX_
