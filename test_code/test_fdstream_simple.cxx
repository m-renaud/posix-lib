#include <iostream>

#include "../fdstream.hxx"
#include "../pipe.hxx"

int main()
{
  try
  {
    mrr::posix::pipe successful_auth;
    mrr::posix::fd_ostream write_end(successful_auth.write_end());
    mrr::posix::fd_istream read_end(successful_auth.read_end());

    int received[2];

    write_end << 5 << std::endl;
    write_end << 8 << std::endl;

    read_end >> received[0];
    read_end >> received[1];

    std::cout << received[0] << std::endl
              << received[1] << std::endl;

    successful_auth.close();
  }
  catch(std::system_error const& except)
  {
    mrr::posix::output_system_error_msg();
    exit(1);
  }

  return 0;

}
