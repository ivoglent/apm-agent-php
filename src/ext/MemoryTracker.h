/*
   +----------------------------------------------------------------------+
   | Elastic APM agent for PHP                                            |
   +----------------------------------------------------------------------+
   | Copyright (c) 2020 Elasticsearch B.V.                                |
   +----------------------------------------------------------------------+
   | Elasticsearch B.V. licenses this file under the Apache 2.0 License.  |
   | See the LICENSE file in the project root for more information.       |
   +----------------------------------------------------------------------+
 */

#pragma once

#include <stdbool.h>
#include "basic_types.h"
#include "StringView.h"
#include "basic_macros.h"
#include "IntrusiveDoublyLinkedList.h"
#include "internal_checks.h"

#ifndef ELASTIC_APM_MEMORY_TRACKING_ENABLED_01
#   if defined( ELASTIC_APM_MEMORY_TRACKING_ENABLED ) && ( ELASTIC_APM_MEMORY_TRACKING_ENABLED == 0 )
#       define ELASTIC_APM_MEMORY_TRACKING_ENABLED_01 0
#   else
#       define ELASTIC_APM_MEMORY_TRACKING_ENABLED_01 1
#   endif
#endif

#if ( ELASTIC_APM_MEMORY_TRACKING_ENABLED_01 != 0 )

enum MemoryTrackingLevel
{
    memoryTrackingLevel_not_set = -1,
    memoryTrackingLevel_off = 0,

    memoryTrackingLevel_totalCountOnly,
    memoryTrackingLevel_eachAllocation,
    memoryTrackingLevel_eachAllocationWithStackTrace,

    memoryTrackingLevel_all,
    numberOfMemoryTrackingLevels = memoryTrackingLevel_all + 1
};
typedef enum MemoryTrackingLevel MemoryTrackingLevel;

#define ELASTIC_APM_ASSERT_VALID_MEMORY_TRACKING_LEVEL( level ) \
    ELASTIC_APM_ASSERT( ELASTIC_APM_IS_IN_END_EXCLUDED_RANGE( memoryTrackingLevel_not_set, (level), numberOfMemoryTrackingLevels ) \
        , #level ": %u", (unsigned int)(level) ) \
/**/

extern const char* memoryTrackingLevelNames[ numberOfMemoryTrackingLevels ];

#ifndef ELASTIC_APM_MEMORY_TRACKING_DEFAULT_LEVEL
#   if ( ELASTIC_APM_IS_DEBUG_BUILD_01 != 0 )
#       define ELASTIC_APM_MEMORY_TRACKING_DEFAULT_LEVEL memoryTrackingLevel_all
#   else
#       define ELASTIC_APM_MEMORY_TRACKING_DEFAULT_LEVEL memoryTrackingLevel_off
#   endif
#endif

#ifndef ELASTIC_APM_MEMORY_TRACKING_DEFAULT_ABORT_ON_MEMORY_LEAK
#   if ( ELASTIC_APM_IS_DEBUG_BUILD_01 != 0 )
#       define ELASTIC_APM_MEMORY_TRACKING_DEFAULT_ABORT_ON_MEMORY_LEAK true
#   else
#       define ELASTIC_APM_MEMORY_TRACKING_DEFAULT_ABORT_ON_MEMORY_LEAK false
#   endif
#endif

MemoryTrackingLevel internalChecksToMemoryTrackingLevel( InternalChecksLevel internalChecksLevel );

struct MemoryTracker
{
    MemoryTrackingLevel level;
    bool abortOnMemoryLeak;

    UInt64 allocatedPersistent;
    UInt64 allocatedRequestScoped;
    IntrusiveDoublyLinkedList allocatedBlocks;
};
typedef struct MemoryTracker MemoryTracker;

static inline
void assertValidMemoryTracker( MemoryTracker* memTracker )
{
    ELASTIC_APM_ASSERT_VALID_PTR( memTracker );
    ELASTIC_APM_ASSERT_VALID_MEMORY_TRACKING_LEVEL( memTracker->level );
    ELASTIC_APM_ASSERT_VALID_INTRUSIVE_LINKED_LIST( &memTracker->allocatedBlocks );
}
ELASTIC_APM_SUPPRESS_UNUSED( assertValidMemoryTracker );

#define ELASTIC_APM_ASSERT_VALID_MEMORY_TRACKER( memTracker ) \
    ELASTIC_APM_ASSERT_VALID_OBJ( assertValidMemoryTracker( memTracker ) ) \

static inline
bool isMemoryTrackingEnabled( MemoryTracker* memTracker )
{
    return memTracker->level > memoryTrackingLevel_off;
}

static inline
bool shouldCaptureStackTrace( MemoryTracker* memTracker )
{
    return memTracker->level > memoryTrackingLevel_eachAllocationWithStackTrace;
}

static inline
void reconfigureMemoryTracker(
        MemoryTracker* memTracker,
        MemoryTrackingLevel newConfiguredLevel,
        bool newConfiguredAbortOnMemoryLeak )
{
    ELASTIC_APM_ASSERT_VALID_MEMORY_TRACKING_LEVEL( newConfiguredLevel );
    ELASTIC_APM_ASSERT( newConfiguredLevel != memoryTrackingLevel_not_set, "" );

    /// We cannot increase tacking level after the start because it's possible that some allocations were already made
    /// so starting tracking with higher level after some allocations were already made will produce invalid results
    memTracker->level = ELASTIC_APM_MIN( memTracker->level, newConfiguredLevel );

    memTracker->abortOnMemoryLeak = newConfiguredAbortOnMemoryLeak;
}

void constructMemoryTracker( MemoryTracker* memTracker );
void memoryTrackerRequestInit( MemoryTracker* memTracker );
size_t memoryTrackerCalcSizeToAlloc(
        MemoryTracker* memTracker,
        size_t originallyRequestedSize,
        size_t stackTraceAddressesCount );
void memoryTrackerAfterAlloc(
        MemoryTracker* memTracker,
        const void* allocatedBlock,
        size_t originallyRequestedSize,
        bool isPersistent,
        size_t actuallyRequestedSize,
        StringView filePath,
        UInt lineNumber,
        bool isString,
        void* const* stackTraceAddresses,
        size_t stackTraceAddressesCount );
void memoryTrackerBeforeFree(
        MemoryTracker* memTracker,
        const void* allocatedBlock,
        size_t originallyRequestedSize,
        bool isPersistent,
        size_t* possibleActuallyRequestedSize );
void memoryTrackerRequestShutdown( MemoryTracker* memTracker );
void destructMemoryTracker( MemoryTracker* memTracker );

struct TextOutputStream;
typedef struct TextOutputStream TextOutputStream;
String streamMemoryTrackingLevel( MemoryTrackingLevel level, TextOutputStream* txtOutStream );

MemoryTracker* getGlobalMemoryTracker();

#endif // #if ( ELASTIC_APM_MEMORY_TRACKING_ENABLED_01 != 0 )
