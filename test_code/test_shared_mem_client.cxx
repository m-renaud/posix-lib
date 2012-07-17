#include <iostream>
#include <string>
#include "../shared_mem.hxx"


int main()
{
  int id;
  int int_val;
  std::string str_val;
  
  mrr::posix::shared_memory<int> num;
  mrr::posix::shared_memory<char, 1000> str;

  std::cout << "Enter int segment id: ";
  std::cin >> id;

  num.attach(id);

  int_val = num;

  std::cout << "Int value = " << int_val << std::endl;

  std::cout << "Enter str segment id: ";
  std::cin >> id;

  str.attach(id);

  str_val = str;

  std::cout << "Str value = " << str_val << std::endl;

  return 0;
}
