#ifndef __rCORE_EEPROM_H__
#define __rCORE_EEPROM_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace neeprom
    {
        u32 crc32(const byte* data, s32 length);
        bool load(byte* data, s32 size);
        void save(byte const* data, s32 size);
    }  // namespace neeprom
}  // namespace ncore

#endif  // __rCORE_EEPROM_H__
