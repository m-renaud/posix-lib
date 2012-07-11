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


template <typename T, unsigned Size = 1>
struct shared_memory
{
  using value_type = T;
  using stripped_ptr_type = typename std::remove_pointer<value_type>::type;
  using flag_type = int;
  
  shared_memory(int flags)
    : segment_id_(shmget(IPC_PRIVATE, sizeof(T)*Size, flags)),
      shared_mem_(static_cast<value_type*>(shmat(segment_id_, NULL, 0)))
  {
  }

  shared_memory()
    : segment_id_(-1), shared_mem_(nullptr)
  {
  }

  shared_memory(shared_memory const& shm)
    : segment_id_(shm.segment_id_), shared_mem_(shm.shared_mem_)
  {
  }

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

  ~shared_memory()
  {
    release();
  }

  shared_memory& operator =(value_type const& val)
  {
    *shared_mem_ = val;
    return *this;
  }

  // For char[]
  shared_memory& operator =(value_type const * const& str)
  {
    sprintf(shared_mem_, str);
    return *this;
  }

  value_type* operator&()
  {
    return shared_mem_;
  }

  std::ostream& print(std::ostream& os) const
  {
    if(Size > 1 && std::is_same<value_type,char>::value)
      os << shared_mem_;
    else
      os << *shared_mem_;

    return os;
  }

  value_type& operator*()
  {
    return *shared_mem_;
  }
     
private:
  int segment_id_;
  value_type* shared_mem_;
  
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
