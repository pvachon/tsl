add_library(tsl STATIC
    assert.c
    cpumask.c
    diag.c
    errors.c
    frame_alloc.c
    hexdump.c
    panic.c
    parsers.c
    printf_helpers.c
    rbtree.c
    safe_alloc.c
    safe_string.c
    threading.c
    time.c
    version.c
    worker_thread.c
    alloc/allocator.c)

set(TSL_HEADERS
    alloc.h
    assert.h
    atomic_arm.h
    basic.h
    bits.h
    cal.h
    coroutine.h
    cpumask.h
    diag.h
    errors.h
    frame_alloc.h
    hexdump.h
    list.h
    notifier.h
    panic.h
    parsers.h
    printf_helpers.h
    rbtree.h
    refcnt.h
    result.h
    safe_alloc.h
    safe_string.h
    sections.h
    speed.h
    threading.h
    ticket_lock.h
    time.h
    time_priv.h
    types.h
    version.h
    worker_thread.h
    work_queue.h
    alloc/alloc_priv.h)

add_executable(version_dump version_dump/version_dump.c)
target_link_libraries(version_dump tsl)
target_include_directories(version_dump PRIVATE "${TSL_BASE_DIR}")

install(TARGETS tsl
    ARCHIVE DESTINATION lib)
install(FILES ${TSL_HEADERS}
    DESTINATION ${TSL_HEADERS_INSTALL_BASE}/tsl)

add_subdirectory(test)
