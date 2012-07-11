#ifndef MRR_POSIX_LIB_SHARED_MEM_HXX_
#define MRR_POSIX_LIB_SHARED_MEM_HXX_

#include <sys/shm.h>
#include <sys/stat.h>


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// TODO:
//
// Break this into 3 partial specializations:
//
// Single Value:
//
// template<typename T>
// struct shared_memory<T,1>;
//
//
// String:
//
// template <unsigned N>
// struct shared_memory<char,N>;
//
//
// Array:
//
// template <typename T, unsigned N>
// struct shared_memory<T,N>;
// 


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace mrr {
namespace posix {

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename T, unsigned Size>
struct shared_memory;


//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename T, unsigned Size>
struct shared_memory_base
{
  using value_type = T;
  using flag_type = int;

private:
  friend class shared_memory<T,Size>;

  
protected:
  shared_memory_base(int flags)
    : segment_id_(shmget(IPC_PRIVATE, sizeof(T)*Size, flags)),
      shared_mem_(static_cast<value_type*>(shmat(segment_id_, NULL, 0)))
  {
  }

  shared_memory_base()
    : segment_id_(-1), shared_mem_(nullptr)
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

  void clear()
  {
    shmdt(shared_mem_);
  }

  void release()
  {
    shmctl(segment_id_, IPC_RMID, NULL);
  }

  ~shared_memory_base()
  {
    release();
  }


protected:
  int segment_id_;
  value_type* shared_mem_;

}; // struct shared_memory base



//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
template <typename T, unsigned Size = 1>
struct shared_memory : shared_memory_base<T,Size>
{
  using base_type = shared_memory_base<T,Size>;

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

  shared_memory& operator =(value_type const& val)
  {
    *base_type::shared_mem_ = val;
    return *this;
  }

  // For char[]
  shared_memory& operator =(value_type const * const& str)
  {
    sprintf(base_type::shared_mem_, str);
    return *this;
  }

  value_type* operator&()
  {
    return base_type::shared_mem_;
  }

  std::ostream& print(std::ostream& os) const
  {
    if(Size > 1 && std::is_same<value_type,char>::value)
      os << base_type::shared_mem_;
    else
      os << *base_type::shared_mem_;

    return os;
  }

  value_type& operator*()
  {
    return *base_type::shared_mem_;
  }
     
}; // struct shared_memory





//m=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

} // namespace posix
} // namespace mrr

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

template <typename T, unsigned Size>
std::ostream& operator <<(std::ostream& os, mrr::posix::shared_memory<T,Size> const& mem)
{
  return mem.print(os);
}




#endif // #ifndef MRR_POSIX_LIB_SHARED_MEM_HXX_
