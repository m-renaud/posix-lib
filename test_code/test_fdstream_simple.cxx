#include <iostream>

#include "../fdstream.hxx"
#include "../pipe.hxx"

int main()
{
  mrr::posix::pipe successful_auth;
  mrr::posix::fd_ostream write_end(successful_auth.write_end());
  mrr::posix::fd_istream read_end(successful_auth.read_end());

  read_end.tie(&write_end);

  int received;

  write_end << 5 << std::endl;
  write_end << 8 << std::endl;

  read_end >> received;
  std::cout << received << std::endl;

  read_end >> received;
  std::cout << received << std::endl;

  return 0;

}
