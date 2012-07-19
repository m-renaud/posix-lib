#include <iostream>
#include <fcntl.h>

#include "../child_process.hxx"
#include "../fdstream.hxx"
#include "../pipe.hxx"
#include "../fd_select.hxx"


void print_number(int fd, int num)
{
  mrr::posix::fd_ostream write(fd);

  sleep(num);
  write << num << std::endl;
}

void read_numbers_impl(int fd)
{
  int num;
  std::cout << "Activity from " << fd << std::endl;
  mrr::posix::fd_istream read(fd);
  read >> num;
  std::cout << "Num = " << num << std::endl;
}

void read_numbers(int fd)
{
  auto c = mrr::posix::make_child_process(read_numbers_impl);
  c.fork(fd);
  c.wait(WNOHANG);
}

int main()
{
  mrr::posix::pipe p1;
  mrr::posix::pipe p2;

  auto c1 = mrr::posix::make_child_process(print_number);
  auto c2 = mrr::posix::make_child_process(print_number);

  c1.fork(p1.write_end(), 1);
  c1.wait(WNOHANG);

  c2.fork(p2.write_end(), 2);
  c2.wait(WNOHANG);

  mrr::posix::fd_select p_select;

  p_select.watch(p1.read_end(), read_numbers);
  p_select.watch(p2.read_end(), read_numbers);
  
  p_select.listen_with_action();
}
