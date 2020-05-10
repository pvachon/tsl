add_executable(test_app test_app.c)

target_link_libraries(test_app PRIVATE
    jansson
    testframework
    app
    config
    tsl
    ck)

target_include_directories(test_app PRIVATE ${PROJECT_SOURCE_DIR})
