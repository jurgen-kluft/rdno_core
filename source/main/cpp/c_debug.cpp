#include "rdno_core/c_debug.h"

#ifdef TARGET_ARDUINO
#    include "Arduino.h"

namespace ncore
{
    struct debug_manager_t
    {
        s32 m_default_level = DEBUG_LEVEL_WARN;
    };

    debug_manager_t  gDebugManagerInstance;
    debug_manager_t* gDebugManager = &gDebugManagerInstance;

    void setTagLevel(debug_manager_t* tm, debug_tag_t& tag, s32 level) { tag.level = level; }
    void setTagToDefaultLevel(debug_manager_t* tm, debug_tag_t& tag) { tag.level = tm->m_default_level; }
#    ifdef TARGET_ESP32
    u32         getFreeHeap(debug_manager_t* tm) { return ESP.getFreeHeap(); }
    u32         getStackHighWaterMark(debug_manager_t* tm) { return uxTaskGetStackHighWaterMark(NULL); }
    const char* getTaskGetName(debug_manager_t* tm) { return pcTaskGetName(NULL); }
    #else
    u32         getFreeHeap(debug_manager_t* tm) { return 0; }
    u32         getStackHighWaterMark(debug_manager_t* tm) { return 0; }
    const char* getTaskGetName(debug_manager_t* tm) { return "UNKNOWN"; }
#    endif

    const char* gTagLevelStr[] = {"NONE", "ERROR", "WARN", "INFO", "DBG", "VERBOSE", "UNKNOWN", "UNKNOWN"};
    const char* getTagLevelStr(debug_manager_t* tm, debug_tag_t const& tag)
    {
        const char* tagLevelStr = gTagLevelStr[getTagLevel(tm, tag) & 0x07];
        return tagLevelStr;
    }

#    ifdef TARGET_ESP8266

    const char* IRAM_ATTR extractFileName(const char* path)
    {
        const char* filename = (const char*)path;
        const char* p        = (const char*)path;
        while (*p)
        {
            if (*p == '/' || *p == '\\')
            {
                filename = p + 1;
            }
            p++;
        }
        return filename;
    }

#    endif  // ESP8266
}  // namespace ncore

#endif
