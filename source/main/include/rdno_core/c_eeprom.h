#ifndef __RDNO_CORE_EEPROM_H__
#define __RDNO_CORE_EEPROM_H__
#include "rdno_core/c_target.h"
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

#endif  // __RDNO_CORE_EEPROM_H__
