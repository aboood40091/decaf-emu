#pragma once
#include <common/structsize.h>
#include <libdecaf/src/cafe/libraries/gx2/gx2_enum.h>

struct GX2RBuffer
{
    void* buffer(const void* basePtr) const
    {
        return (void*)((uintptr_t)basePtr + bufferOffset);
    }

   cafe::gx2::GX2RResourceFlags flags;
   uint32_t elemSize;
   uint32_t elemCount;
   uint32_t bufferOffset; // void*
};
CHECK_SIZE(GX2RBuffer, 0x10);
CHECK_OFFSET(GX2RBuffer, 0x00, flags);
CHECK_OFFSET(GX2RBuffer, 0x04, elemSize);
CHECK_OFFSET(GX2RBuffer, 0x08, elemCount);
CHECK_OFFSET(GX2RBuffer, 0x0C, bufferOffset);
