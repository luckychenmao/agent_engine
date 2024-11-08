
#pragma once

#include <assert.h>
#include <iosfwd>
#include <string.h>

#include "util/CommonMacros.h"
#include "util/mem_pool/Pool.h"

#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#endif

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/rapidjson.h"

namespace util {

class UtilPoolAllocator {
public:
    static const bool kNeedFree = false;

    UtilPoolAllocator(util::mem_pool::Pool *pool_ = NULL) {
        if (pool_) {
            pool = pool_;
            ownPool = false;
        } else {
            pool = new util::mem_pool::Pool;
            ownPool = true;
        }
    }
    ~UtilPoolAllocator() {
        if (ownPool) {
            DELETE_AND_SET_NULL(pool);
        }
    }

private:
    UtilPoolAllocator(const UtilPoolAllocator &);
    UtilPoolAllocator &operator=(const UtilPoolAllocator &);

public:
    void *Malloc(size_t size) {
        if (size == 0) {
            return NULL;
        } else {
            return pool->allocate(size);
        }
    }
    void *Realloc(void *originalPtr, size_t originalSize, size_t newSize) {
        // do nothing to old mem
        if (originalPtr == 0) {
            return Malloc(newSize);
        }
        if (newSize == 0) {
            return NULL;
        }
        if (originalSize >= newSize) {
            return originalPtr;
        }
        void *newBuffer = Malloc(newSize);
        assert(newBuffer != 0);
        if (originalSize)
            std::memcpy(newBuffer, originalPtr, originalSize);
        return newBuffer;
    }
    static void Free(void *ptr) {
        // do nothing to old mem
    }

public:
    util::mem_pool::Pool *pool;
    bool ownPool;
};

typedef rapidjson::GenericValue<rapidjson::UTF8<>, UtilPoolAllocator> SimpleValue;
typedef rapidjson::GenericDocument<rapidjson::UTF8<>, UtilPoolAllocator> SimpleDocument;

} // namespace util
