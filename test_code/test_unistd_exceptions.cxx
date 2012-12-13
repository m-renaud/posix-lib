#include <iostream>
#include "../unistd.hxx"
#include "../cxx_system_error.hxx"

int main()
{
  std::cout << "==================================================\n";
  try
  {
    // open()
    int fd = mrr::posix::open("test_unistd_exceptions.cxx", O_RDONLY);
    std::cout << "fd = " << fd << std::endl;

    // read()
    char buffer[BUFSIZ];
    while(mrr::posix::read(fd, buffer, BUFSIZ))
      std::cout << buffer << std::endl;

    // close()
    mrr::posix::close(14);

  }
  catch(std::system_error const& except)
  {
    mrr::posix::output_system_error_msg();
    exit(1);
  }

  std::cout << "==================================================\n";
  try
  {
    // open()
    int fd = mrr::posix::open("output_test", O_WRONLY | O_CREAT | O_TRUNC);
    std::cout << "fd = " << fd << std::endl;

    // write()
    std::string str = "hello there\n";
    mrr::posix::write(fd, str.data(), str.size());

    // close()
    mrr::posix::close(fd);

  }
  catch(std::system_error const& except)
  {
    mrr::posix::output_system_error_msg();
    exit(1);
  }

  return 0;
}
