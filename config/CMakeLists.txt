add_library(config STATIC
    engine.c
    util.c)

set(TSL_CONFIG_HEADERS
    engine.h
    util.h)

target_include_directories(config INTERFACE "${PROJECT_SOURCE_DIR}")
target_include_directories(config PUBLIC
    "${PROJECT_BUILD_DIR}"
    "${JANSSON_INCLUDE_DIRS}"
    "${PROJECT_SOURCE_DIR}"
    "${CK_INCLUDE_DIRS}")

add_subdirectory(test)

install(TARGETS config
    ARCHIVE DESTINATION lib)
install(FILES ${TSL_CONFIG_HEADERS}
    DESTINATION ${TSL_HEADERS_INSTALL_BASE}/config)

