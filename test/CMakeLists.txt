add_library(tsltestframework STATIC
    assert.c
    filesystem.c
    framework.c
    main.c
    malloc_helpers.c)

set(TSL_TESTFRAMEWORK_HEADERS
    assert.h
    filesystem.h
    framework.h
    framework_priv.h
    malloc_helpers.h)

target_include_directories(tsltestframework INTERFACE "${PROJECT_SOURCE_DIR}")
target_include_directories(tsltestframework PUBLIC
                           "${PROJECT_BUILD_DIR}"
                           "${JANSSON_INCLUDE_DIRS}"
                           "${PROJECT_SOURCE_DIR}"
                           "${CK_INCLUDE_DIRS}")

install(TARGETS tsltestframework
    ARCHIVE DESTINATION lib)
install(FILES ${TSL_TESTFRAMEWORK_HEADERS}
    DESTINATION ${TSL_HEADERS_INSTALL_BASE}/test)

