#include <iostream>

#include "fdstream.hxx"
#include "file_descriptor.hxx"
#include "pipe.hxx"

struct opener
{
  int port;
  unsigned seconds;

  opener()
  {
  }
  
  opener(int p, unsigned s)
    : port(p), seconds(s)
  {
  }
  
};

std::ostream& operator<<(std::ostream& os, opener const& message)
{
  os << message.port << ' ' << message.seconds;
  return os;
}

std::istream& operator >>(std::istream& is, opener& message)
{
  is >> message.port >> message.seconds;
  return is;
}

int main()
{
  mrr::posix::pipe successful_auth;
  mrr::posix::fd_ostream write_end(successful_auth.write_end());
  mrr::posix::fd_istream read_end(successful_auth.read_end());

  read_end.tie(&write_end);

  int received;

#if 1
  write_end << 5;
#else
  write_end << 5 << std::char_traits<char>::eof();
#endif

  write_end.flush();

  read_end >> received;

  std::cout << received << std::endl;

  return 0;

}
