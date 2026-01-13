#pragma once

// ===================================================================================
//
// ObjectPool.h (最終相容性版本)
//
// 這個版本使用 unsigned char 來代替 std::byte，
// 以解決 std::byte 與舊版 Windows SDK 中自定義的 byte 型別之間的命名衝突。
//
// ===================================================================================

#include <cstddef>  // 為了 size_t 和 alignof
#include <new>
#include <stack>
#include <utility>
#include <vector>

template <typename T>
class ObjectPool {
 private:
  // 使用 unsigned char 來代表原始記憶體，與 C++14 及更早版本相容
  std::vector<unsigned char> m_memory_block;

  std::stack<void*> m_freeList;

 public:
  explicit ObjectPool(size_t initialSize) {
    if (initialSize == 0) return;

    size_t object_size_aligned =
        (sizeof(T) + alignof(T) - 1) & ~(alignof(T) - 1);
    m_memory_block.resize(initialSize * object_size_aligned);

    for (size_t i = 0; i < initialSize; ++i) {
      void* chunk_address = m_memory_block.data() + (i * object_size_aligned);
      m_freeList.push(chunk_address);
    }
  }

  ObjectPool(const ObjectPool&) = delete;
  ObjectPool& operator=(const ObjectPool&) = delete;

  template <typename... Args>
  T* acquire(Args&&... args) {
    if (m_freeList.empty()) {
      return nullptr;
    }

    void* memory_chunk = m_freeList.top();
    m_freeList.pop();

    T* obj = new (memory_chunk) T(std::forward<Args>(args)...);
    return obj;
  }

  void release(T* obj) {
    if (obj) {
      obj->~T();
      m_freeList.push(obj);
    }
  }
};