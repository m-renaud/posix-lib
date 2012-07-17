#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include "../shared_mem.hxx"


int main()
{
  mrr::posix::shared_memory<int> num(S_IRUSR | S_IWUSR);
  mrr::posix::shared_memory<char,1000> str(S_IRUSR | S_IWUSR);

  std::cout << "Int Segment id: " << num.segment_id() << std::endl;
  std::cout << "Str Segment id: " << str.segment_id() << std::endl;

  num = 5;

  str = "hello there!!!!";

  std::cerr << "Back in main\n";

  std::cerr << "The string is: " << str << std::endl;

  mrr::posix::shared_memory<int,10> array(S_IRUSR | S_IWUSR);

  for(auto& x : array)
    x = 5;

  std::cout << '[';
  for(auto const& x : array)
    std::cout << ' ' << x;
  std::cout << " ]" << std::endl;

  std::transform(
    begin(array), end(array),
    std::ostream_iterator<int>(std::cout, "\n"),
    [](int x)
    {
      return x * 5;
    }
  );

  for(;;)
    ;

  return 0;
}
