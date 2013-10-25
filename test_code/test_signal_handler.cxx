#include <iostream>
#include <functional>
#include <memory>

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
	// std::unique_ptr<int> is;
};

int main()
{
	foo f1;

	mrr::posix::signals.set_handle(SIGINT, [&]{f1.func_to_handle_sigint();});
	mrr::posix::signals.ignore(SIGHUP);

	for(;;)
		;
}
