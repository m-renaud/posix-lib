#ifndef MRR_POSIX_LIB_SHARED_MEM_HXX_
#define MRR_POSIX_LIB_SHARED_MEM_HXX_

#include <sys/shm.h>
#include <sys/stat.h>

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename T, std::size_t N>
struct shared_memory;


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename T, std::size_t N>
struct shared_memory_base
{
  using value_type = T;
  using flag_type = int;

private:
  friend class shared_memory<T,N>;


protected:
  shared_memory_base()
    : segment_id_(-1), shared_mem_(nullptr), auto_release(false)
  {
  }

  shared_memory_base(int flags)
    : segment_id_(shmget(IPC_PRIVATE, sizeof(T)*N, flags)),
      shared_mem_(static_cast<value_type*>(shmat(segment_id_, NULL, 0))),
      auto_release(true)
  {
  }


public:
  int segment_id() const
  {
    return segment_id_;
  }

  void attach(int segment_id)
  {
    segment_id_ = segment_id;
    shared_mem_ = static_cast<value_type*>(shmat(segment_id_, NULL, 0));
  }

  void detatch()
  {
    shmdt(shared_mem_);
    segment_id_ = -1;
    shared_mem_ = nullptr;
  }

  void release()
  {
    if(shared_mem_ != nullptr)
    {
      int seg_id = segment_id_;
      this->detatch();
      shmctl(seg_id, IPC_RMID, NULL);
    }
  }

  ~shared_memory_base()
  {
    if(auto_release)
      release();
  }


protected:
  int segment_id_;
  value_type* shared_mem_;
  bool auto_release;

}; // struct shared_memory base



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Partial specialization for an array of type T
//
template <typename T, std::size_t N = 1>
struct shared_memory : shared_memory_base<T,N>
{
  using base_type = shared_memory_base<T,N>;

  using value_type = typename base_type::value_type;
  using flag_type = typename base_type::flag_type;

  using size_type = std::size_t;
  using iterator = T*;

  shared_memory(int flags)
    : base_type(flags)
  {
  }

  shared_memory()
    : base_type()
  {
  }

  shared_memory(shared_memory const& shm)
    : base_type(shm)
  {
  }

  iterator begin()
  {
    return base_type::shared_mem_;
  }

  iterator end()
  {
    return base_type::shared_mem_ + N;
  }

  std::ostream& print(std::ostream& os) const
  {
    os << base_type::shared_mem_;
    return os;
  }

  value_type& operator [](std::size_t n)
  {
    return *(base_type::shared_mem_ + n);
  }

}; // struct shared_memory<T,N> - array



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Partial specialization for a single type
//
template <typename T>
struct shared_memory<T,1> : shared_memory_base<T,1>
{
  using base_type = shared_memory_base<T,1>;

  using value_type = typename base_type::value_type;
  using flag_type = typename base_type::flag_type;

  shared_memory(int flags)
    : base_type(flags)
  {
  }

  shared_memory()
    : base_type()
  {
  }

  shared_memory(shared_memory const& shm)
    : base_type(shm)
  {
  }


  shared_memory& operator =(value_type const& val)
  {
    *base_type::shared_mem_ = val;
    return *this;
  }

  std::ostream& print(std::ostream& os) const
  {
    os << *base_type::shared_mem_;
    return os;
  }

  operator value_type&()
  {
    return *base_type::shared_mem_;
  }

}; // struct shared_memory<T,1> - single type




//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Partial specialization for a string
//
template <std::size_t N>
struct shared_memory<char,N> : shared_memory_base<char,N>
{
  using base_type = shared_memory_base<char,N>;

  using value_type = typename base_type::value_type;
  using flag_type = typename base_type::flag_type;

  using size_type = std::size_t;
  using iterator = value_type*;

  shared_memory(int flags)
    : base_type(flags)
  {
  }

  shared_memory()
    : base_type()
  {
  }

  shared_memory(shared_memory const& shm)
    : base_type(shm)
  {
  }

  shared_memory& operator =(char const*const str)
  {
    sprintf(base_type::shared_mem_, str);
    return *this;
  }

  std::ostream& print(std::ostream& os) const
  {
    os << base_type::shared_mem_;
    return os;
  }

  iterator begin()
  {
    return base_type::shared_mem_;
  }

  iterator end()
  {
    return base_type::shared_mem_ + N;
  }

  operator std::string()
  {
    return std::string(base_type::shared_mem_);
  }

  value_type& operator [](std::size_t n)
  {
    return *(base_type::shared_mem_ + n);
  }

}; // struct shared_memory<char,N> - string



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Corner case for a single char
//
template <>
struct shared_memory<char,1> : shared_memory_base<char,1>
{
  using base_type = shared_memory_base<char,1>;

  using value_type = typename base_type::value_type;
  using flag_type = typename base_type::flag_type;

  shared_memory(int flags)
    : base_type(flags)
  {
  }

  shared_memory()
    : base_type()
  {
  }

  shared_memory(shared_memory const& shm)
    : base_type(shm)
  {
  }


  shared_memory& operator =(value_type const& val)
  {
    *base_type::shared_mem_ = val;
    return *this;
  }

  std::ostream& print(std::ostream& os) const
  {
    os << *base_type::shared_mem_;
    return os;
  }

  operator value_type&()
  {
    return *base_type::shared_mem_;
  }

}; // struct shared_memory<char,1> - corner case


template <typename T, std::size_t N>
inline auto begin(shared_memory<T,N>& shm) -> decltype(shm.begin())
{
  return shm.begin();
}

template <typename T, std::size_t N>
inline auto end(shared_memory<T,N>& shm) -> decltype(shm.end())
{
  return shm.end();
}


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

template <typename T, std::size_t N>
std::ostream& operator <<(std::ostream& os, mrr::posix::shared_memory<T,N> const& mem)
{
  return mem.print(os);
}



#endif // #ifndef MRR_POSIX_LIB_SHARED_MEM_HXX_
