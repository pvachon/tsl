add_executable(test_config test_engine.c)

target_link_libraries(test_config PRIVATE
    jansson
    testframework
    app
    config
    tsl
    ck)

target_include_directories(test_config PRIVATE ${PROJECT_SOURCE_DIR})

