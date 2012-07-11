#include <iostream>
#include <string>
#include "shared_mem.hxx"


int main()
{
  mrr::posix::shared_memory<int> num(S_IRUSR | S_IWUSR);
  mrr::posix::shared_memory<char,1000> str(S_IRUSR | S_IWUSR);

  std::cout << "Int Segment id: " << num.segment_id() << std::endl;
  std::cout << "Str Segment id: " << str.segment_id() << std::endl;

  num = 5;
  // sprintf(&str, "Hello there :)");
  str = "hello there!!!!";

  std::cerr << "Back in main\n";

  std::cerr << "The string is: " << str << std::endl;

  for(;;)
    ;

  return 0;
}
