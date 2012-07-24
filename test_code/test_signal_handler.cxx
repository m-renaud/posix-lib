#include <iostream>
#include <functional>

#include "../signal_handler.hxx"


struct foo
{
  foo()
    : i()
  {
  }

  void func_to_handle_sigint()
  {
    std::cout << "Handling SIGINT with i = " << ++i << std::endl;
  }

  int i;
};

int main()
{
  foo f1;

  mrr::posix::signals.set_handle(SIGINT, std::bind(&foo::func_to_handle_sigint, f1));
  mrr::posix::ignore(SIGHUP);

  for(;;)
    ;
}
