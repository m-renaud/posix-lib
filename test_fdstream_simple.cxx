#include <iostream>

#include "fdstream.hxx"
#include "pipe.hxx"

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
