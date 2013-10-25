#include <iostream>
#include <functional>

#include "../child_process.hxx"

void sum(int a, int b)
{
  std::cout << a << " + " << b << " = " << a+b << std::endl;
}

int main()
{
  auto p1 = mrr::posix::make_child_process(
    [](int a) { std::cout << "a = " << a << std::endl; }
  );

  p1.fork(5);
  p1.wait();

  auto p2 = mrr::posix::make_child_process(sum);

  p2.fork(5,10);
  p2.wait();

  auto prebound = std::bind(sum, 50, 11);

  auto p3 = mrr::posix::make_child_process(prebound);

  p3.fork();
  p3.wait();

}
