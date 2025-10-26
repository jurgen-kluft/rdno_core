#include "rdno_core/c_eeprom.h"
#include "rdno_core/c_memory.h"
#include "rdno_core/c_serial.h"

#ifdef TARGET_ARDUINO

#    include "Arduino.h"
#    include "EEPROM.h"

namespace ncore
{
    namespace neeprom
    {
        u32 crc32(const byte* data, s32 length)
        {
            int i, j;
            u32 byte, crc, mask;

            i   = 0;
            crc = 0xFFFFFFFF;
            while (i < length)
            {
                byte = data[i];  // Get next byte.
                crc  = crc ^ byte;
                for (j = 7; j >= 0; j--)
                {  // Do eight times.
                    mask = -(crc & 1);
                    crc  = (crc >> 1) ^ (0xEDB88320 & mask);
                }
                i = i + 1;
            }
            return ~crc;
        }

        void save(byte const* data, s32 size)
        {
            EEPROM.begin(512);
            for (s32 i = 0; i < size; i++)
                EEPROM.write(i, data[i]);
            EEPROM.commit();
        }

        bool load(byte* data, s32 size)
        {
            EEPROM.begin(512);
            for (s32 i = 0; i < size; i++)
                data[i] = EEPROM.read(i);
            return true;
        }

    }  // namespace neeprom
}  // namespace ncore

#else

namespace ncore
{
    namespace neeprom
    {
        u32 crc32(const byte* data, s32 length) { return 0; }
        bool load(byte* data, s32 size) { return false; }
        void save(byte const* data, s32 size) {}

    }  // namespace neeprom
}  // namespace ncore

#endif
