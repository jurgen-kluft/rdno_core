#ifndef __rCORE_DEBUG_H__
#define __rCORE_DEBUG_H__
#include "rcore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
#define DEBUG_LEVEL_NONE    0
#define DEBUG_LEVEL_ERROR   1
#define DEBUG_LEVEL_WARN    2
#define DEBUG_LEVEL_INFO    3
#define DEBUG_LEVEL_DBG     4
#define DEBUG_LEVEL_VERBOSE 5

#if defined(TARGET_ESP32)
#    define NCORE_DEBUG_LEVEL CORE_DEBUG_LEVEL
#endif

#if defined(TARGET_ESP8266)
#    define NCORE_DEBUG_LEVEL CORE_DEBUG_LEVEL
#endif

    /*
    Usage Example:

        DEBUG_TAG(my_tag, "MyTagName");

        void my_function()
        {
            DEBUG_INFO(my_tag, "This is an info message");
            DEBUG_ERROR(my_tag, "This is an error message");
        }

    */

    struct debug_manager_t;
    extern debug_manager_t* gDebugManager;

    struct debug_tag_t
    {
#if NCORE_DEBUG_LEVEL > DEBUG_LEVEL_NONE
        debug_tag_t()
            : tag("undefined")
            , level(DEBUG_LEVEL_WARN)
        {
        }
        debug_tag_t(const char* t, s32 l)
            : tag(t)
            , level(l)
        {
        }
        const char* tag;
        s32         level;
#endif
    };

#define DEBUG_TAG(name, tag_descr) ncore::debug_tag_t name = ncore::debug_tag_t(tag_descr, DEBUG_LEVEL_WARN)

#if NCORE_DEBUG_LEVEL > DEBUG_LEVEL_NONE
    void        setTagLevel(debug_manager_t* tm, debug_tag_t& tag, s32 level);
    void        setTagToDefaultLevel(debug_manager_t* tm, debug_tag_t& tag);
    s32         getTagLevel(debug_manager_t* tm, debug_tag_t const& tag) { return tag.level; }
    const char* getTagLevelStr(debug_manager_t* tm, debug_tag_t const& tag);
    u32         getFreeHeap(debug_manager_t* tm);
    u32         getStackHighWaterMark(debug_manager_t* tm);
    const char* getTaskGetName(debug_manager_t* tm);
#else
    void        setTagLevel(ncore::debug_manager_t* tm, ncore::debug_tag_t const& tag, s32 level) {}
    void        setTagToDefaultLevel(ncore::debug_manager_t* tm, ncore::debug_tag_t const& tag) {}
    s32         getTagLevel(ncore::debug_manager_t* tm, ncore::debug_tag_t const& tag) { return DEBUG_LEVEL_NONE; }
    const char* getTagLevelStr(ncore::debug_manager_t* tm, ncore::debug_tag_t const& tag) { return "UNKNOWN"; }
    u32         getFreeHeap(ncore::debug_manager_t* tm) { return 0; }
    u32         getStackHighWaterMark(ncore::debug_manager_t* tm) { return 0; }
    const char* getTaskGetName(ncore::debug_manager_t* tm) { return "unknown"; }
#endif

#define SetTagDebugLevel(tag, level)   ncore::setTagLevel(ncore::gDebugManager, tag, level)
#define SetTagToDefaultDebugLevel(tag) ncore::setTagToDefaultLevel(ncore::gDebugManager, tag)
#define GetTagDebugLevel(tag)          ncore::getTagLevel(ncore::gDebugManager, tag)
#define GetTagDebugLevelStr(tag)       ncore::getTagLevelStr(ncore::gDebugManager, tag)

#ifdef TARGET_ESP8266
#    ifndef CONFIG_ARDUHAL_LOG_COLORS
#        define CONFIG_ARDUHAL_LOG_COLORS 0
#    endif

#    if CONFIG_ARDUHAL_LOG_COLORS
#        define ARDUHAL_LOG_COLOR_BLACK   "30"
#        define ARDUHAL_LOG_COLOR_RED     "31"  // ERROR
#        define ARDUHAL_LOG_COLOR_GREEN   "32"  // INFO
#        define ARDUHAL_LOG_COLOR_YELLOW  "33"  // WARNING
#        define ARDUHAL_LOG_COLOR_BLUE    "34"
#        define ARDUHAL_LOG_COLOR_MAGENTA "35"
#        define ARDUHAL_LOG_COLOR_CYAN    "36"  // DEBUG
#        define ARDUHAL_LOG_COLOR_GRAY    "37"  // VERBOSE
#        define ARDUHAL_LOG_COLOR_WHITE   "38"

#        define ARDUHAL_LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#        define ARDUHAL_LOG_BOLD(COLOR)  "\033[1;" COLOR "m"
#        define ARDUHAL_LOG_RESET_COLOR  "\033[0m"

#        define ARDUHAL_LOG_COLOR_E             ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_RED)
#        define ARDUHAL_LOG_COLOR_W             ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_YELLOW)
#        define ARDUHAL_LOG_COLOR_I             ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_GREEN)
#        define ARDUHAL_LOG_COLOR_D             ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_CYAN)
#        define ARDUHAL_LOG_COLOR_V             ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_GRAY)
#        define ARDUHAL_LOG_COLOR_PRINT(letter) log_printf(ARDUHAL_LOG_COLOR_##letter)
#        define ARDUHAL_LOG_COLOR_PRINT_END     log_printf(ARDUHAL_LOG_RESET_COLOR)
#    else
#        define ARDUHAL_LOG_COLOR_E
#        define ARDUHAL_LOG_COLOR_W
#        define ARDUHAL_LOG_COLOR_I
#        define ARDUHAL_LOG_COLOR_D
#        define ARDUHAL_LOG_COLOR_V
#        define ARDUHAL_LOG_RESET_COLOR
#        define ARDUHAL_LOG_COLOR_PRINT(letter)
#        define ARDUHAL_LOG_COLOR_PRINT_END
#    endif
#endif  // ESP8266

#ifdef TARGET_ESP8266

#    define DEBUG_ESP_PORT Serial

#    ifdef DEBUG_ESP_PORT

    const char* extractFileName(const char* path);

#        define DEBUG_LINE_PREFIX(TAG)                                                                                                                                                  \
            DEBUG_ESP_PORT.printf(PSTR("[%6lu][H:%5lu][%s:%d] %s(): ["), millis(), ncore::getFreeHeap(ncore::gDebugManager), ncore::extractFileName(__FILE__), __LINE__, __FUNCTION__); \
            DEBUG_ESP_PORT.print(TAG.tag);                                                                                                                                              \
            DEBUG_ESP_PORT.print("] ");

#        if NCORE_DEBUG_LEVEL >= DEBUG_LEVEL_VERBOSE
#            define DEBUG_VERBOSE(TAG, text, ...)                                    \
                if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_VERBOSE) \
                {                                                                    \
                    DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_V "V ");                  \
                    DEBUG_LINE_PREFIX(TAG);                                          \
                    DEBUG_ESP_PORT.printf(PSTR(text), ##__VA_ARGS__);                \
                    DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);                 \
                }
#        else
#            define DEBUG_VERBOSE(...)
#        endif

#        if NCORE_DEBUG_LEVEL >= DEBUG_LEVEL_DBG
#            define DEBUG_DBG(TAG, text, ...)                                    \
                if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_DBG) \
                {                                                                \
                    DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_D "D ");              \
                    DEBUG_LINE_PREFIX(TAG);                                      \
                    DEBUG_ESP_PORT.printf(PSTR(text), ##__VA_ARGS__);            \
                    DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);             \
                }
#        else
#            define DEBUG_DBG(...)
#        endif

#        if NCORE_DEBUG_LEVEL >= DEBUG_LEVEL_INFO
#            define DEBUG_INFO(TAG, text, ...)                                    \
                if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_INFO) \
                {                                                                 \
                    DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_I "I ");               \
                    DEBUG_LINE_PREFIX(TAG);                                       \
                    DEBUG_ESP_PORT.printf(PSTR(text), ##__VA_ARGS__);             \
                    DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);              \
                }
#        else
#            define DEBUG_INFO(...)
#        endif

#        if NCORE_DEBUG_LEVEL >= DEBUG_LEVEL_WARN
#            define DEBUG_WARN(TAG, text, ...)                                    \
                if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_WARN) \
                {                                                                 \
                    DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_W "W ");               \
                    DEBUG_LINE_PREFIX(TAG);                                       \
                    DEBUG_ESP_PORT.printf(PSTR(text), ##__VA_ARGS__);             \
                    DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);              \
                }
#        else
#            define DEBUG_WARN(...)
#        endif

#        if NCORE_DEBUG_LEVEL >= DEBUG_LEVEL_ERROR
#            define DEBUG_ERROR(TAG, text, ...)                                    \
                if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_ERROR) \
                {                                                                  \
                    DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_E "E ");                \
                    DEBUG_LINE_PREFIX(TAG);                                        \
                    DEBUG_ESP_PORT.printf(PSTR(text), ##__VA_ARGS__);              \
                    DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);               \
                }
#        else
#            define DEBUG_ERROR(...)
#        endif

#    else
#        define enableDebugOutput(...)
#        define DEBUG_VERBOSE(...)
#        define DEBUG_DBG(...)
#        define DEBUG_INFO(...)
#        define DEBUG_WARN(...)
#        define DEBUG_ERROR(...)
#    endif

#endif

#ifdef TARGET_ESP32

#    define DEBUG_ESP_PORT Serial

#    ifdef GET_TASK_STACK_INFO
#        define DEBUG_VERBOSE(TAG, format, ...)                                                                                                                                                                            \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_VERBOSE)                                                                                                                                           \
            {                                                                                                                                                                                                              \
                ESP_LOGV(TAG.tag, "[H:%6d][S:%d T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getStackHighWaterMark(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_DBG(TAG, format, ...)                                                                                                                                                                                \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_DBG)                                                                                                                                               \
            {                                                                                                                                                                                                              \
                ESP_LOGD(TAG.tag, "[H:%6d][S:%d T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getStackHighWaterMark(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_INFO(TAG, format, ...)                                                                                                                                                                               \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_INFO)                                                                                                                                              \
            {                                                                                                                                                                                                              \
                ESP_LOGI(TAG.tag, "[H:%6d][S:%d T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getStackHighWaterMark(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_WARN(TAG, format, ...)                                                                                                                                                                               \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_WARN)                                                                                                                                              \
            {                                                                                                                                                                                                              \
                ESP_LOGW(TAG.tag, "[H:%6d][S:%d T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getStackHighWaterMark(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_ERROR(TAG, format, ...)                                                                                                                                                                              \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_ERROR)                                                                                                                                             \
            {                                                                                                                                                                                                              \
                ESP_LOGE(TAG.tag, "[H:%6d][S:%d T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getStackHighWaterMark(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#    else
#        define DEBUG_VERBOSE(TAG, format, ...)                                                                                                                   \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_VERBOSE)                                                                                  \
            {                                                                                                                                                     \
                ESP_LOGV(TAG.tag, "[H:%6d][T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_DBG(TAG, format, ...)                                                                                                                       \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_DBG)                                                                                      \
            {                                                                                                                                                     \
                ESP_LOGD(TAG.tag, "[H:%6d][T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_INFO(TAG, format, ...)                                                                                                                      \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_INFO)                                                                                     \
            {                                                                                                                                                     \
                ESP_LOGI(TAG.tag, "[H:%6d][T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_WARN(TAG, format, ...)                                                                                                                      \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_WARN)                                                                                     \
            {                                                                                                                                                     \
                ESP_LOGW(TAG.tag, "[H:%6d][T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#        define DEBUG_ERROR(TAG, format, ...)                                                                                                                     \
            if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_ERROR)                                                                                    \
            {                                                                                                                                                     \
                ESP_LOGE(TAG.tag, "[H:%6d][T:%s] " format, ncore::getFreeHeap(ncore::gDebugManager), ncore::getTaskGetName(ncore::gDebugManager), ##__VA_ARGS__); \
            }
#    endif  // GET_TASK_STACK_INFO
#endif      // TARGET_ESP32

#define LOG_ERROR_IF_NON_ZERO(TAG, ERROR_CODE, format, ...)                   \
    if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_ERROR)        \
    {                                                                         \
        if (ERROR_CODE != 0)                                                  \
        {                                                                     \
            DEBUG_ERROR(TAG, "Code: %d. " format, ERROR_CODE, ##__VA_ARGS__); \
        }                                                                     \
    }
#define LOG_ERROR_IF_ZERO(TAG, ERROR_CODE, format, ...)                \
    if (ncore::getTagLevel(ncore::gDebugManager, TAG) >= DEBUG_LEVEL_ERROR) \
    {                                                                  \
        if (ERROR_CODE == 0)                                           \
        {                                                              \
            DEBUG_ERROR(TAG.tag, "Code: 0. " format, ##__VA_ARGS__);   \
        }                                                              \
    }

#define LOG_IF_CODE(ERROR_LEVEL, TAG, ERROR_CODE, CODE, format, ...)                  \
    if (ERROR_CODE == CODE)                                                           \
    {                                                                                 \
        DEBUG_##ERROR_LEVEL(TAG.tag, "Code: %d. " format, ERROR_CODE, ##__VA_ARGS__); \
    }

}  // namespace ncore

#endif
