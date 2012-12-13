#include "../fdstream.hxx"
#include "../pipe.hxx"

struct foo
{
  foo() = default;
  
  foo(int a_)
    : a(a_)
  {
  }
  
  int a;
};

std::istream& operator >>(std::istream& is, foo& f)
{
  char c;
  is >> c >> f.a >> c;
  return is;
}

std::ostream& operator <<(std::ostream& os, foo const& f)
{
  os << '{' << f.a << '}';
  return os;
}

int main()
{
  mrr::posix::pipe successful_auth;
  mrr::posix::fd_ostream write_end(successful_auth.write_end().get_fd());
  mrr::posix::fd_istream read_end(successful_auth.read_end().get_fd());

  read_end.tie(&write_end);

  foo received;

  write_end << foo(5) << std::endl;

  read_end >> received;
  std::cout << received.a << std::endl;

  return 0;
}
