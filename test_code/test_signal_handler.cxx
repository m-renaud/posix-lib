#include <iostream>
#include <functional>

#include "../signal_handler.hxx"


struct foo
{
  void func_to_handle_sigint()
  {
    std::cout << "Handling SIGINT with acces to all instance variables :)\n";
  }
};

int main()
{
  foo f1;
  mrr::posix::signals.set_handle(SIGINT, std::bind(&foo::func_to_handle_sigint, f1));

  for(;;)
    ;
}
