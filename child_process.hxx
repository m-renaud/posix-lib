#ifndef MRR_CHILD_PROCESS_HXX_
#define MRR_CHILD_PROCESS_HXX_

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

struct fork_failed
{
};

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename Function>
struct child_process
{
  using function_type = Function;
  using pid_type = pid_t;
  using status_type = int;

  child_process(function_type const& func)
    : func_(func), pid_(), status_(), waited_for(false)
  {
  }

  ~child_process()
  {
    this->wait();
  }

  template <typename... FuncArgs>
  void fork(FuncArgs&&... args)
  {
    pid_ = ::fork();
    if(pid_ == 0)
    {
      func_(std::forward<FuncArgs>(args)...);
      std::exit(0);
    }
  }
  
  void wait(int options = 0)
  {
    if(pid_ != 0 && !waited_for)
    {
      ::waitpid(pid_, &status_, options);
      waited_for = true;
    }
  }

  pid_type pid() const
  {
    return pid_;
  }

  status_type status() const
  {
    return status_;
  }


private:
  function_type func_;
  pid_type pid_;
  status_type status_;
  bool waited_for;
  
}; // struct child_process<Function>


template <typename Function>
inline child_process<Function> make_child_process(Function&& func)
{
  return child_process<Function>(std::forward<Function>(func));
  
} // child_process<Function> make_child_process()

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


#endif // #ifndef MRR_CHILD_PROCESS_HXX_
