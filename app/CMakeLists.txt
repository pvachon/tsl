add_library(tslapp STATIC
    app.c
    cpufeatures.c)

set(TSL_APP_HEADERS
    app.h
    cpufeatures.h
    )

target_include_directories(tslapp INTERFACE "${PROJECT_SOURCE_DIR}")
target_include_directories(tslapp PUBLIC
                           "${PROJECT_BUILD_DIR}"
                           "${JANSSON_INCLUDE_DIRS}"
                           "${PROJECT_SOURCE_DIR}"
                           "${CK_INCLUDE_DIRS}")

install(TARGETS tslapp
    ARCHIVE DESTINATION lib)
install(FILES ${TSL_APP_HEADERS}
    DESTINATION ${TSL_HEADERS_INSTALL_BASE}/app)

add_subdirectory(test)
