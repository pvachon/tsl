/*
 *  cpufeatures.c - x86 CPU Feature Checks
 *
 *  Copyright (c)2013-17 Phil Vachon <phil@security-embedded.com>
 *
 *  This file is a part of The Standard Library (TSL)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <app/cpufeatures.h>
#include <app/app.h>

#include <tsl/errors.h>
#if defined(__x86_64__)
#include <tsl/diag.h>
#include <tsl/assert.h>
#include <tsl/cal.h>

#include <stdbool.h>


#define CPUID_LEAF_FEATURE_FLAGS            0x1

/**
 * The check for the CMPXCHG16B instruction
 */
#define CPUID_FEATURE_CMPXCHG16B            13

/**
 * SSE 4.2 includes the crc32 instruction
 */
#define CPUID_FEATURE_SSE_42                20

/**
 * The check for LZCNT and POPCNT availability
 */
#define CPUID_FEATURE_POPCNT                23

/**
 * We use AVX, so that is also required
 */
#define CPUID_FEATURE_AVX                   28

/**
 * Base of features in EDX
 */
#define CPUID_BASE_FEATURE_EDX              32

/**
 * Macro to that does the math for CPUID features in EDX
 */
#define CPUID_FEATURE_EDX(x)                ((x) + CPUID_BASE_FEATURE_EDX)

/**
 * Presence of the TSC
 */
#define CPUID_FEATURE_TSC                   CPUID_FEATURE_EDX(4)

#define CPU_MSG(sev, sys, msg, ...) MESSAGE("CPUFEATURE", sev, sys, msg, ##__VA_ARGS__)

static
void __cpuid(uint32_t leaf, uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx)
{
    *eax = leaf;
    *ecx = 2;

    __asm__ __volatile__ (
        "\tcpuid\n"
        : "+a"(*eax), "=b"(*ebx), "+c"(*ecx), "=d"(*edx)
        : : "memory"
    );
}

static
aresult_t __cpuid_check_feature(unsigned feature_id, bool *available)
{
    aresult_t ret = A_OK;

    uint32_t flags[2];
    uint32_t junk = 0;

    TSL_ASSERT_ARG(feature_id < 64);

    __cpuid(CPUID_LEAF_FEATURE_FLAGS, &junk, &junk, &flags[0], &flags[1]);

    if (feature_id >= 32) {
        *available = !!(flags[1] & (1ul << (feature_id - 32)));
    } else {
        *available = !!(flags[0] & (1ul << feature_id));
    }

    return ret;
}

#endif /* defined(__x86_64__) */

aresult_t app_cpufeatures_check_at_init(void)
{
    aresult_t ret = A_OK;

#if defined(__x86_64__)
    bool result = true,
         feature = false;

    DIAG("Checking CPU Feature Support");

    TSL_BUG_IF_FAILED(__cpuid_check_feature(CPUID_FEATURE_CMPXCHG16B, &feature));
    result &= feature;
    if (false == feature) {
        CPU_MSG(SEV_FATAL, "MISSING-CPUID-FEATURE", "This application requires CMPXCHG16B be present and enabled.");
    }

#ifdef _TSL_NEED_SSE_42
    TSL_BUG_IF_FAILED(__cpuid_check_feature(CPUID_FEATURE_SSE_42, &feature));
    result &= feature;
    if (false == feature) {
        CPU_MSG(SEV_FATAL, "MISSING-CPUID-FEATURE", "This application requires SSE 4.2 be present and enabled.");
    }
#endif

#ifdef _TSL_NEED_AVX
    TSL_BUG_IF_FAILED(__cpuid_check_feature(CPUID_FEATURE_AVX, &feature));
    result &= feature;
    if (false == feature) {
        CPU_MSG(SEV_FATAL, "MISSING-CPUID-FEATURE", "This application requires AVX be present and enabled.");
    }
#endif

    TSL_BUG_IF_FAILED(__cpuid_check_feature(CPUID_FEATURE_POPCNT, &feature));
    result &= feature;
    if (false == feature) {
        CPU_MSG(SEV_FATAL, "MISSING-CPUID-FEATURE", "This application requires POPCNT be available.");
    }

    TSL_BUG_IF_FAILED(__cpuid_check_feature(CPUID_FEATURE_TSC, &feature));
    result &= feature;
    if (false == feature) {
        CPU_MSG(SEV_FATAL, "MISSING-CPUID-FEATURE", "This application requires the Time Stamp Counter and RDTSC be available.");
    }

    if (false == result) {
        PANIC("Failed CPU feature checks, aborting. Please file an issue with the contents of /proc/cpuinfo on github.com/pvachon/tsl");
    }
#endif /* defined(__x86_64__) */

    return ret;
}

