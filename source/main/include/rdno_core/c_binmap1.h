#ifndef __RDNO_CORE_BINMAP_V2_H__
#define __RDNO_CORE_BINMAP_V2_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    class alloc_t;

    // Note: Size has to be passed to each function since most of the time

    // 2^8 binmap, can handle a maximum of 4 * 64 = 256 bits.
    struct binmap8_t
    {
        u64 m_bin0[4];
    };

    void g_setup_free(binmap8_t* bt);
    void g_setup_used(binmap8_t* bt);
    void g_clear(binmap8_t* bt);

    void g_set(binmap8_t* bt, u32 const maxbits, u32 bit);
    void g_clr(binmap8_t* bt, u32 const maxbits, u32 bit);
    bool g_get(binmap8_t* bt, u32 const maxbits, u32 bit);
    s32  g_find(binmap8_t* bt, u32 const maxbits);
    s32  g_find_and_set(binmap8_t* bt, u32 const maxbits);

    // 2^12 binmap, can handle a maximum of 4096 bits.
    struct binmap12_t
    {
        u64  m_bin0;
        u64* m_bin1;
    };

    void g_setup(alloc_t* allocator, binmap12_t* bt, u32 const maxbits);

    void g_setup_free_lazy(alloc_t* allocator, binmap12_t* bt, u32 const maxbits);
    void g_tick_free_lazy(binmap12_t* bt, u32 const maxbits, u32 bit);
    void g_setup_used_lazy(alloc_t* allocator, binmap12_t* bt, u32 const maxbits);
    void g_tick_used_lazy(binmap12_t* bt, u32 const maxbits, u32 bit);

    void g_clear(alloc_t* allocator, binmap12_t* bt, u32 const maxbits);
    void g_release(alloc_t* allocator, binmap12_t* bt);

    void g_set(binmap12_t* bt, u32 const maxbits, u32 bit);
    void g_clr(binmap12_t* bt, u32 const maxbits, u32 bit);
    bool g_get(binmap12_t* bt, u32 const maxbits, u32 bit);
    s32  g_find(binmap12_t* bt, u32 const maxbits);
    s32  g_find_and_set(binmap12_t* bt, u32 const maxbits);


}  // namespace ncore

#endif
