#include <limits>

#include "CUDACore/allocate_host.h"
#include "CUDACore/cudaCheck.h"

#include "getCachingHostAllocator.h"

namespace {
  //const size_t maxAllocationSize = allocator::intPow(cms::cuda::allocator::binGrowth, cms::cuda::allocator::maxBin);
  const size_t maxAllocationSize = notcub::CachingDeviceAllocator::IntPow(cms::cuda::allocator::binGrowth, cms::cuda::allocator::maxBin);
}

namespace cms::cuda {
  void *allocate_host(size_t nbytes, cudaStream_t stream) {
    void *ptr = nullptr;
    if constexpr (allocator::useCaching) {
      if (nbytes > maxAllocationSize) {
        throw std::runtime_error("Tried to allocate " + std::to_string(nbytes) +
                                 " bytes, but the allocator maximum is " + std::to_string(maxAllocationSize));
      }
      cudaCheck(allocator::getCachingHostAllocator().HostAllocate(&ptr, nbytes, stream));      
      //ptr = allocator::getCachingHostAllocator().allocate(nbytes, stream);
    } else {
      cudaCheck(cudaMallocHost(&ptr, nbytes));
    }
    return ptr;
  }

  void free_host(void *ptr) {
    if constexpr (allocator::useCaching) {
	cudaCheck(allocator::getCachingHostAllocator().HostFree(ptr));
      } else {
      cudaCheck(cudaFreeHost(ptr));
    }
  }
  /*
  void free_host(void *ptr) {
    if constexpr (allocator::policy == allocator::Policy::Caching) {
      allocator::getCachingHostAllocator().free(ptr);
    } else {
      cudaCheck(cudaFreeHost(ptr));
    }
  }
  */
}  // namespace cms::cuda
