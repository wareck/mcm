/*	MCM file compressor

  Copyright (C) 2013, Google Inc.
  Authors: Mathieu Chartier

  LICENSE

    This file is part of the MCM file compressor.

    MCM is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MCM is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MCM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <cstring>

#include "Memory.hpp"
#include "Util.hpp"

#include <new>

#define USE_MALLOC 1

#ifdef _WIN32
#include <windows.h>
#else
// TODO: mmap
#endif

MemMap::MemMap() : storage(nullptr), size(0) {

}

MemMap::~MemMap() {
  release();
}

void MemMap::resize(size_t bytes) {
  if (bytes == size) {
    std::fill(reinterpret_cast<uint8_t*>(storage), reinterpret_cast<uint8_t*>(storage) + size, 0);
    return;
  }
  release();
  size = bytes;
#if USE_MALLOC
  storage = std::calloc(1, bytes);
#elif defined(_WIN32)
  storage = (void*)VirtualAlloc(nullptr, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
#error UNIMPLEMENTED
#endif
  if (storage == nullptr && bytes != 0) {
    // Report instead of crashing later on a null pointer (e.g. a high memory level
    // like -x10 on a machine or a 32-bit process without enough memory).
    size = 0;
    throw std::bad_alloc();
  }
}

void MemMap::release() {
  if (storage != nullptr) {
#if USE_MALLOC
    std::free(storage);
#elif defined(_WIN32)
    BOOL result = VirtualFree((LPVOID)storage, size, MEM_DECOMMIT);
#else
#error UNIMPLEMENTED
#endif
    storage = nullptr;
  }
}

void MemMap::zero() {
#if USE_MALLOC
  if (storage != nullptr) std::memset(storage, 0, size);
#elif defined(_WIN32)
  storage = (void*)VirtualAlloc(storage, size, MEM_RESET, PAGE_READWRITE);
#else
  madvise(storage, size, MADV_DONTNEED);
#endif
}
