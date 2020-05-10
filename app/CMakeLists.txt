add_library(app STATIC
    app.c
    cpufeatures.c)

set(TSL_APP_HEADERS
    app.h
    cpufeatures.h
    )

target_include_directories(app INTERFACE "${PROJECT_SOURCE_DIR}")
target_include_directories(app PUBLIC
                           "${PROJECT_BUILD_DIR}"
                           "${JANSSON_INCLUDE_DIRS}"
                           "${PROJECT_SOURCE_DIR}"
                           "${CK_INCLUDE_DIRS}")

install(TARGETS app
    ARCHIVE DESTINATION lib)
install(FILES ${TSL_APP_HEADERS}
    DESTINATION ${TSL_HEADERS_INSTALL_BASE}/app)

add_subdirectory(test)
